#ifndef RIO_GFX_MDL_MESH_H
#define RIO_GFX_MDL_MESH_H

#include <gfx/mdl/res/rio_MeshData.h>
#include <gpu/rio_VertexArray.h>
#include <math/rio_Matrix.h>

namespace rio { namespace mdl {

class Material;
class Model;

class Mesh
{
public:
    Mesh(const res::Mesh* res_mesh, Model* parent_mdl);

    const res::Mesh& resMesh() const
    {
        return mResMesh;
    }

    Model& parentModel() const
    {
        return mParentModel;
    }

    Material* material() const
    {
        return mMaterial;
    }

    const Matrix34f& worldMtx() const
    {
        return mWorldMtx;
    }

    void draw() const;

private:
    void setMaterial_(Material* material);
    void calcLocalMtx_();
    void calcWorldMtx_(const Matrix34f& mdl_world_mtx);

private:
    const res::Mesh&    mResMesh;           // Mesh resource.
    Model&              mParentModel;       // Parent Model.

    Material*           mMaterial;          // Material to use.

    Matrix34f           mLocalMtx;          // Local transformation matrix.
    Matrix34f           mWorldMtx;          // World transformation matrix. (Model x Local)

    const u32*          mIdxBuf;            // Index buffer.
    u32                 mIdxNum;            // Indices count.

    VertexBuffer        mVBO;               // Vertex buffer object.
    VertexStream        mPosStream;         // Position vertex attribute stream.
    VertexStream        mTexCoordStream;    // Texture coordinates vertex attribute stream layout.
    VertexStream        mNormalStream;      // Normal vertex attribute stream.
    VertexArray         mVAO;               // Vertex array object.

    friend class Model;
};

} }

#endif // RIO_GFX_MDL_MESH_H
