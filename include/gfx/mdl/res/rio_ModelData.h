#ifndef RIO_GFX_MDL_RES_MODEL_DATA_H
#define RIO_GFX_MDL_RES_MODEL_DATA_H

#include <gfx/mdl/res/rio_MaterialData.h>
#include <gfx/mdl/res/rio_MeshData.h>

namespace rio { namespace mdl { namespace res {

class Material;
class Mesh;
class ModelCacher;

class Model
{
public:
    // Minimum supported version
    static constexpr u32 cVersionMin     = 0x01000000;
    // Current version
    static constexpr u32 cVersionCurrent = 0x01000000;

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

private:
    char                mMagic[8]; // "riomodel"
    u32                 mVersion;   // File version
    u32                 mFileSize;  // File size in bytes
    Buffer<Mesh>        mMeshes;    // List of meshes
    Buffer<Material>    mMaterials; // List of materials

    friend class ModelCacher;
};
static_assert(std::is_standard_layout<Model>::value && std::is_trivial<Model>::value);
static_assert(sizeof(Model) == 0x20);

} } }

#endif // RIO_GFX_MDL_RES_MODEL_DATA_H
