#ifndef RIO_GFX_MDL_RES_MODEL_DATA_H
#define RIO_GFX_MDL_RES_MODEL_DATA_H

#include <gfx/mdl/res/rio_MaterialData.h>
#include <gfx/mdl/res/rio_MeshData.h>
#include <gfx/mdl/res/rio_SkeletonData.h>

namespace rio { namespace mdl { namespace res {

class Material;
class Mesh;
class ModelCacher;

class Model
{
public:
    // Minimum supported version
    static constexpr u32 cVersionMin     = 0x02000000;
    // Current version
    static constexpr u32 cVersionCurrent = 0x02000000;

public:
    u32 numMeshes() const
    {
        return mMeshes.count();
    }

    const Mesh* meshes() const
    {
        return mMeshes.ptr();
    }

    const Mesh& mesh(u32 i) const
    {
        RIO_ASSERT(i < numMeshes());
        return mMeshes.ptr()[i];
    }

    u32 numMaterials() const
    {
        return mMaterials.count();
    }

    const Material* materials() const
    {
        return mMaterials.ptr();
    }

    const Material& material(u32 i) const
    {
        RIO_ASSERT(i < numMaterials());
        return mMaterials.ptr()[i];
    }

    const Skeleton& skeleton() const
    {
        return mSkeleton;
    }

    u32 numSkeletalAnimations() const
    {
        return mSklAnims.count();
    }

    const res::SkeletalAnimation* skeletalAnimations() const
    {
        return mSklAnims.ptr();
    }

    const res::SkeletalAnimation& skeletalAnimation(u32 i) const
    {
        RIO_ASSERT(i < numSkeletalAnimations());
        return mSklAnims.ptr()[i];
    }

private:
    char                        mMagic[8];  // "riomodel"
    u32                         mVersion;   // File version
    u32                         mFileSize;  // File size in bytes
    Buffer<Mesh>                mMeshes;    // List of meshes
    Buffer<Material>            mMaterials; // List of materials
    Skeleton                    mSkeleton;  // Skeleton
    Buffer<SkeletalAnimation>   mSklAnims;  // Skeletal animations

    friend class ModelCacher;
};
static_assert(std::is_pod<Model>::value);
static_assert(sizeof(Model) == 0x34);

} } }

#endif // RIO_GFX_MDL_RES_MODEL_DATA_H
