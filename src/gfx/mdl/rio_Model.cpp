#include <gfx/mdl/rio_Material.h>
#include <gfx/mdl/rio_Mesh.h>
#include <gfx/mdl/rio_Model.h>
#include <misc/rio_MemUtil.h>

#include <new>

namespace rio { namespace mdl {

Model::Model(const res::Model* res_mdl)
    : mResModel(*res_mdl)
    , mMeshes(nullptr)
    , mMaterials(nullptr)
    , mSkeleton(&res_mdl->skeleton())
    , mModelMtx{Matrix34f::ident}
{
    RIO_ASSERT(res_mdl);

    mNumMeshes = mResModel.numMeshes();
    if (mNumMeshes > 0)
    {
        mMeshes = (Mesh*)MemUtil::alloc(sizeof(Mesh) * mNumMeshes, 4);
        const res::Mesh* const meshes = mResModel.meshes();

        for (u32 i = 0; i < mNumMeshes; i++)
        {
            new (&mMeshes[i]) Mesh(&meshes[i], this);
            mMeshes[i].calcBoneBaseTransform(mSkeleton);
        }
    }

    mNumMaterials = mResModel.numMaterials();
    if (mNumMaterials > 0)
    {
        mMaterials = (Material*)MemUtil::alloc(sizeof(Material) * mNumMaterials, 4);
        const res::Material* const materials = mResModel.materials();

        for (u32 i = 0; i < mNumMaterials; i++)
            new (&mMaterials[i]) Material(&materials[i], this);
    }

    for (u32 i = 0; i < mNumMeshes; i++)
    {
        Mesh& mesh = mMeshes[i];
        u32 mat_idx = mesh.resMesh().materialIndex();

        RIO_ASSERT(mat_idx < mNumMaterials);
        Material& material = mMaterials[mat_idx];

        mesh.setMaterial_(&material);
        material.pushBackMesh_(&mesh);
    }
}

Model::~Model()
{
    if (mNumMeshes > 0)
    {
        for (u32 i = 0; i < mNumMeshes; i++)
            mMeshes[i].~Mesh();

        MemUtil::free(mMeshes);
    }

    if (mNumMaterials > 0)
    {
        for (u32 i = 0; i < mNumMaterials; i++)
            mMaterials[i].~Material();

        MemUtil::free(mMaterials);
    }
}

void Model::setModelWorldMtx(const Matrix34f& srt)
{
    mModelMtx = srt;

    for (u32 i = 0; i < mNumMeshes; i++)
        mMeshes[i].calcWorldMtx_(srt);
}

s32 Model::getSkeletalAnimationIndex(const std::string& name)
{
    for (u32 i = 0; i < mResModel.numSkeletalAnimations(); i++)
        if (name == mResModel.skeletalAnimations()[i].name())
            return i;

    return -1;
}

void Model::applyAnim(f32 time, s32 skl_anim_idx)
{
    const res::SkeletalAnimation& skl_anim = mResModel.skeletalAnimation(skl_anim_idx);

    for (u32 i = 0; i < mNumMeshes; i++)
        applyAnim_(mMeshes[i], time, skl_anim);
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

    p_quat->setInterpolate(
        keys.ptr()[idx].value(),
        keys.ptr()[idx + 1].value(),
        factor
    );
    p_quat->normalize();
}

static void applyAnim(f32 frame, Mesh& mesh, const res::SkeletalAnimation& skl_anim, const Bone& bone, const rio::Matrix34f& parent_transform)
{
    rio::Matrix34f world_transform;

    if (bone.boneAnim().find(anim_name) == bone.boneAnim().end())
        world_transform.setMul(parent_transform, bone.nodeTransform());

    else
    {
        const BoneAnim& bone_anim = bone.boneAnim().at(anim_name);

        aiVector3D scale;
        calcLerp(&scale, frame, bone_anim.scaling_key);

        aiQuaternion rotate;
        calcLerp(&rotate, frame, bone_anim.rotation_key);

        aiVector3D translate;
        calcLerp(&translate, frame, bone_anim.position_key);

        rio::Matrix44f node_transform;
        new ((void*)node_transform.a) aiMatrix4x4(scale, rotate, translate);

        world_transform.setMul(parent_transform, *(const rio::Matrix34f*)node_transform.a);
    }

    s32 idx = entry.getBoneInstanceIndex(bone.name());
    if (idx != -1)
        entry.setBoneTransform(idx, world_transform, entry.boneInstance(idx).offset_mtx);

    for (const Bone* p_child_bone : bone.children())
        applyAnim(frame, entry, anim_name, *p_child_bone, world_transform);
}

void Model::applyAnim_(Mesh& mesh, f32 time, const res::SkeletalAnimation& skl_anim)
{
    if (!mSkeleton.root())
        return;

    frame = std::fmod(time * skl_anim.fps(), animation.duration());
    ::applyAnim(frame, mesh, skl_anim, *mSkeleton.root(), rio::Matrix34f::ident);
}

} }
