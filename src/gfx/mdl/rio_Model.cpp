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

void Model::applyAnim_(Mesh& mesh, f32 time, const res::SkeletalAnimation& skl_anim)
{
    if (!mSkeleton.root())
        return;

    f32 frame = std::fmod(time * skl_anim.fps(), skl_anim.duration());
    mesh.applyAnim(frame, skl_anim, mSkeleton, *mSkeleton.root(), rio::Matrix34f::ident);
}

} }
