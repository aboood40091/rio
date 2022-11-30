#ifndef RIO_GFX_MDL_MODEL_H
#define RIO_GFX_MDL_MODEL_H

#include <gfx/mdl/res/rio_ModelData.h>
#include <gfx/mdl/rio_Mesh.h>
#include <gfx/mdl/rio_Material.h>
#include <math/rio_Matrix.h>

namespace rio { namespace mdl {

class Material;
class Mesh;

class Model
{
public:
    Model(const res::Model* res_mdl);
    ~Model();

    const res::Model& resModel() const
    {
        return mResModel;
    }

    u32 numMeshes() const
    {
        return mNumMeshes;
    }

    Mesh* meshes() const
    {
        return mMeshes;
    }

    Mesh& mesh(u32 i) const
    {
        RIO_ASSERT(i < numMeshes());
        return mMeshes[i];
    }

    u32 numMaterials() const
    {
        return mNumMaterials;
    }

    Material* materials() const
    {
        return mMaterials;
    }

    Material& material(u32 i) const
    {
        RIO_ASSERT(i < numMaterials());
        return mMaterials[i];
    }

    const Matrix34f& getModelWorldMtx() const { return mModelMtx; }
    void setModelWorldMtx(const Matrix34f& srt);

private:
    const res::Model& mResModel;

    Mesh* mMeshes;
    u32 mNumMeshes;

    Material* mMaterials;
    u32 mNumMaterials;

    Matrix34f mModelMtx;
};

} }

#endif // RIO_GFX_MDL_MODEL_H
