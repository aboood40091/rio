#include <gfx/mdl/rio_Material.h>
#include <gfx/mdl/rio_Mesh.h>
#include <gfx/mdl/rio_Model.h>
#include <gpu/rio_Drawer.h>

namespace rio { namespace mdl {

Mesh::Mesh(const res::Mesh* res_mesh, Model* parent_mdl)
    : mResMesh(*res_mesh)
    , mParentModel(*parent_mdl)
{
    RIO_ASSERT(parent_mdl && res_mesh);

    mIdxBuf = mResMesh.indexBuffer().ptr();
    mIdxNum = mResMesh.indexBuffer().count();

    VertexBuffer::invalidateCache(mIdxBuf, mResMesh.indexBuffer().size());

    mVBO.setStride(sizeof(res::Vertex));
    mVBO.setDataInvalidate(mResMesh.vertexBuffer().ptr(), mResMesh.vertexBuffer().size());

    mPosStream.setLayout(0, VertexStream::FORMAT_32_32_32_FLOAT, offsetof(res::Vertex, pos));
    mTexCoordStream.setLayout(1, VertexStream::FORMAT_32_32_FLOAT, offsetof(res::Vertex, tex_coord));
    mNormalStream.setLayout(2, VertexStream::FORMAT_32_32_32_FLOAT, offsetof(res::Vertex, normal));

    mVAO.addAttribute(mPosStream, mVBO);
    mVAO.addAttribute(mTexCoordStream, mVBO);
    mVAO.addAttribute(mNormalStream, mVBO);
    mVAO.process();

    calcLocalMtx_();
    calcWorldMtx_(Matrix34f::ident);
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
