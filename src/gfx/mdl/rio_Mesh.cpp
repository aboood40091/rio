#include <gfx/mdl/rio_Material.h>
#include <gfx/mdl/rio_Mesh.h>
#include <gfx/mdl/rio_Model.h>
#include <gpu/rio_Drawer.h>
#include <misc/rio_MemUtil.h>

namespace rio { namespace mdl {

Mesh::Mesh(const res::Mesh* res_mesh, Model* parent_mdl)
    : mResMesh(*res_mesh)
    , mParentModel(*parent_mdl)
    , mPositionAttrib(ATTRIB_POSITION)
    , mTexCoordAttrib(ATTRIB_TEX_COORD)
    , mNormalAttrib(ATTRIB_NORMAL)
    , mBlendIndexAttrib(ATTRIB_BLEND_INDEX)
    , mBlendWeightAttrib(ATTRIB_BLEND_WEIGHT)
    , mBoneTransform(nullptr)
{
    RIO_ASSERT(parent_mdl && res_mesh);

    mVtxNum = mResMesh.positionBuffer().count();
    RIO_ASSERT(mVtxNum > 0);

    mIdxBuf = mResMesh.indexBuffer().ptr();
    mIdxNum = mResMesh.indexBuffer().count();

    VertexBuffer::invalidateCache(mIdxBuf, mResMesh.indexBuffer().size());

    mPositionAttrib.vtx_buf.setStride(sizeof(rio::BaseVec3f));
    mPositionAttrib.vtx_buf.setDataInvalidate(mResMesh.positionBuffer().ptr(), mResMesh.positionBuffer().size());
    mPositionAttrib.vtx_stream.setLayout(ATTRIB_POSITION, rio::VertexStream::FORMAT_32_32_32_FLOAT, 0);
    mVAO.addAttribute(mPositionAttrib.vtx_stream, mPositionAttrib.vtx_buf);

    if (mResMesh.texCoordBuffer().size() > 0)
    {
        RIO_ASSERT(mVtxNum == mResMesh.texCoordBuffer().count());
        mTexCoordAttrib.vtx_buf.setStride(sizeof(rio::BaseVec2f));
        mTexCoordAttrib.vtx_buf.setDataInvalidate(mResMesh.texCoordBuffer().ptr(), mResMesh.texCoordBuffer().size());
        mTexCoordAttrib.vtx_stream.setLayout(ATTRIB_TEX_COORD, rio::VertexStream::FORMAT_32_32_FLOAT, 0);
        mVAO.addAttribute(mTexCoordAttrib.vtx_stream, mTexCoordAttrib.vtx_buf);
    }

    if (mResMesh.normalBuffer().size() > 0)
    {
        RIO_ASSERT(mVtxNum == mResMesh.normalBuffer().count());
        mNormalAttrib.vtx_buf.setStride(sizeof(rio::BaseVec3f));
        mNormalAttrib.vtx_buf.setDataInvalidate(mResMesh.normalBuffer().ptr(), mResMesh.normalBuffer().size());
        mNormalAttrib.vtx_stream.setLayout(ATTRIB_NORMAL, rio::VertexStream::FORMAT_32_32_32_FLOAT, 0);
        mVAO.addAttribute(mNormalAttrib.vtx_stream, mNormalAttrib.vtx_buf);
    }

    if (mResMesh.blendIndexBuffer().size() > 0)
    {
        RIO_ASSERT(mVtxNum == mResMesh.blendIndexBuffer().count());
        mBlendIndexAttrib.vtx_buf.setStride(sizeof(rio::BaseVec4i));
        mBlendIndexAttrib.vtx_buf.setDataInvalidate(mResMesh.blendIndexBuffer().ptr(), mResMesh.blendIndexBuffer().size());
        mBlendIndexAttrib.vtx_stream.setLayout(ATTRIB_BLEND_INDEX, rio::VertexStream::FORMAT_32_32_32_32_SINT, 0);
        mVAO.addAttribute(mBlendIndexAttrib.vtx_stream, mBlendIndexAttrib.vtx_buf);
    }

    if (mResMesh.blendWeightBuffer().size() > 0)
    {
        RIO_ASSERT(mVtxNum == mResMesh.blendWeightBuffer().count());
        mBlendWeightAttrib.vtx_buf.setStride(sizeof(rio::BaseVec4f));
        mBlendWeightAttrib.vtx_buf.setDataInvalidate(mResMesh.blendWeightBuffer().ptr(), mResMesh.blendWeightBuffer().size());
        mBlendWeightAttrib.vtx_stream.setLayout(ATTRIB_BLEND_WEIGHT, rio::VertexStream::FORMAT_32_32_32_32_FLOAT, 0);
        mVAO.addAttribute(mBlendWeightAttrib.vtx_stream, mBlendWeightAttrib.vtx_buf);
    }

    mVAO.process();

    calcLocalMtx_();
    calcWorldMtx_(Matrix34f::ident);

    if (mResMesh.numBones() > 0)
        mBoneTransform = static_cast<rio::Matrix34f*>(rio::MemUtil::alloc(sizeof(rio::Matrix34f) * mResMesh.numBones(), rio::Drawer::cUniformBlockAlignment));
}

void Mesh::calcBoneBaseTransform(const Skeleton& skeleton)
{
    if (skeleton.root() && mBoneTransform)
        calcBoneBaseTransform_(skeleton, *skeleton.root(), rio::Matrix34f::ident);
}

s32 Mesh::getMeshBoneIndex(const Skeleton& skeleton, const Bone& bone)
{
    u32 idx = skeleton.getBoneIndex(bone);

    for (u32 i = 0; i < mResMesh.numBones(); i++)
        if (mResMesh.bones()[i].skeletonBoneIndex() == u32(idx))
            return i;

    return -1;
}

void Mesh::calcBoneBaseTransform_(const Skeleton& skeleton, const Bone& bone, const rio::Matrix34f& parent_transform)
{
    rio::Matrix34f world_transform;
    world_transform.setMul(parent_transform, bone.getNodeTransform());

    s32 idx = getMeshBoneIndex(skeleton, bone);
    if (idx != -1)
        mBoneTransform[idx].setMul(world_transform, static_cast<const rio::Matrix34f&>(mResMesh.bones()[idx].offsetMatrix()));

    for (const Bone* p_child_bone : bone.children())
        calcBoneBaseTransform_(skeleton, *p_child_bone, world_transform);
}

static void calcLerp(rio::Vector3f* p_vec, f32 frame, const res::Buffer<res::KeyFrameVec3>& keys)
{
    u32 key_num = keys.count();
    RIO_ASSERT(key_num > 0);
    if (key_num == 1)
    {
        *p_vec = keys.ptr()[0].value();
        return;
    }

    u32 idx = 0;
    for (u32 i = 0; i < key_num - 1; i++)
    {
        if (f32(keys.ptr()[i + 1].frame()) > frame)
        {
            idx = i;
            break;
        }
    }

    const f32 t1 = keys.ptr()[idx].frame();
    const f32 t2 = keys.ptr()[idx + 1].frame();
    const f32 factor = (frame - t1) / (t2 - t1);

    *p_vec = keys.ptr()[idx].value()     * (1 - factor) +
             keys.ptr()[idx + 1].value() * factor;
}

static void calcLerp(rio::Quatf* p_quat, f32 frame, const res::Buffer<res::KeyFrameQuat>& keys)
{
    u32 key_num = keys.count();
    RIO_ASSERT(key_num > 0);
    if (key_num == 1)
    {
        *p_quat = keys.ptr()[0].value();
        return;
    }

    u32 idx = 0;
    for (u32 i = 0; i < key_num - 1; i++)
    {
        if (f32(keys.ptr()[i + 1].frame()) > frame)
        {
            idx = i;
            break;
        }
    }

    const f32 t1 = keys.ptr()[idx].frame();
    const f32 t2 = keys.ptr()[idx + 1].frame();
    const f32 factor = (frame - t1) / (t2 - t1);

    p_quat->setSlerp(
        keys.ptr()[idx].value(),
        keys.ptr()[idx + 1].value(),
        factor
    );
    p_quat->normalize();
}

static const res::BoneAnim* getBoneAnim(const res::SkeletalAnimation& skl_anim, const Skeleton& skeleton, const Bone& bone)
{
    u32 bone_idx = skeleton.getBoneIndex(bone);
    for (u32 i = 0; i < skl_anim.numBoneAnims(); i++)
    {
        const res::BoneAnim& bone_anim = skl_anim.boneAnims()[i];
        if (bone_anim.skeletonBoneIndex() == bone_idx)
            return &bone_anim;
    }

    return nullptr;
}

void Mesh::applyAnim(f32 frame, const res::SkeletalAnimation& skl_anim, const Skeleton& skeleton, const Bone& bone, const rio::Matrix34f& parent_transform)
{
    rio::Matrix34f world_transform;

    const res::BoneAnim* p_bone_anim = getBoneAnim(skl_anim, skeleton, bone);
    if (!p_bone_anim)
        world_transform.setMul(parent_transform, bone.getNodeTransform());

    else
    {
        rio::Vector3f scale;
        calcLerp(&scale, frame, p_bone_anim->scalingKey());

        rio::Quatf rotate;
        calcLerp(&rotate, frame, p_bone_anim->rotationKey());

        rio::Vector3f translate;
        calcLerp(&translate, frame, p_bone_anim->positionKey());

        rio::Matrix34f node_transform;
        node_transform.makeSQT(scale, rotate, translate);

        world_transform.setMul(parent_transform, node_transform);
    }

    s32 idx = getMeshBoneIndex(skeleton, bone);
    if (idx != -1)
        mBoneTransform[idx].setMul(world_transform, static_cast<const rio::Matrix34f&>(mResMesh.bones()[idx].offsetMatrix()));

    for (const Bone* p_child_bone : bone.children())
        applyAnim(frame, skl_anim, skeleton, *p_child_bone, world_transform);
}

void Mesh::draw() const
{
    mVAO.bind();
    Drawer::DrawElements(Drawer::TRIANGLES, mIdxNum, mIdxBuf);
}

void Mesh::setMaterial_(Material* material)
{
    mMaterial = material;
}

void Mesh::calcLocalMtx_()
{
    mLocalMtx.makeSRT(
        mResMesh.scale(),
        mResMesh.rotation(),
        mResMesh.translation()
    );
}

void Mesh::calcWorldMtx_(const Matrix34f& mdl_world_mtx)
{
    mWorldMtx.setMul(mdl_world_mtx, mLocalMtx);
}

} }
