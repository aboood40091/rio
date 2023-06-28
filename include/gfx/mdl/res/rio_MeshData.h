#ifndef RIO_GFX_MDL_RES_MESH_DATA_H
#define RIO_GFX_MDL_RES_MESH_DATA_H

#include <gfx/mdl/res/rio_ModelCommonData.h>
#include <math/rio_Vector.h>

namespace rio { namespace mdl { namespace res {

struct Vertex
{
    BaseVec3f   pos;
    BaseVec2f   tex_coord;
    BaseVec3f   normal;
};
static_assert(std::is_standard_layout<Vertex>::value && std::is_trivial<Vertex>::value);
static_assert(sizeof(Vertex) == 0x20);

typedef Buffer<Vertex> VertexBuffer;

class Mesh
{
public:
    const VertexBuffer& vertexBuffer() const
    {
        return mVtxBuf;
    }

    const BufferU32& indexBuffer() const
    {
        return mIdxBuf;
    }

    const Vector3f& scale() const
    {
        return mScale;
    }

    const Vector3f& rotation() const
    {
        return mRotate;
    }

    const Vector3f& translation() const
    {
        return mTranslate;
    }

    u32 materialIndex() const
    {
        return mMatIdx;
    }

private:
    VertexBuffer    mVtxBuf;    // Vertex buffer
    BufferU32       mIdxBuf;    // Index buffer

    Vector3f        mScale;     // Mesh local scale
    Vector3f        mRotate;    // Mesh local rotation
    Vector3f        mTranslate; // Mesh local translation

    u32             mMatIdx;    // Material index
};
static_assert(std::is_standard_layout<Mesh>::value && std::is_trivial<Mesh>::value);
static_assert(sizeof(Mesh) == 0x38);

} } }

#endif // RIO_GFX_MDL_RES_MESH_DATA_H
