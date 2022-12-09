#ifndef RIO_GFX_MDL_RES_MESH_DATA_H
#define RIO_GFX_MDL_RES_MESH_DATA_H

#include <gfx/mdl/res/rio_ModelCommonData.h>
#include <math/rio_Vector.h>

namespace rio { namespace mdl { namespace res {

class MeshBone
{
public:
    u32 skeletonBoneIndex() const
    {
        return mSklBoneIndex;
    }

    const rio::BaseMtx34f& offsetMatrix() const
    {
        return mOffsetMtx;
    }

private:
    u32 mSklBoneIndex;
    rio::BaseMtx34f mOffsetMtx;
};
static_assert(std::is_pod<MeshBone>::value);
static_assert(sizeof(MeshBone) == 0x34);

typedef Buffer<MeshBone> MeshBoneBuf;

class Mesh
{
public:
    const BufferVec3f& positionBuffer() const
    {
        return mPositionBuf;
    }

    const BufferVec2f& texCoordBuffer() const
    {
        return mTexCoordBuf;
    }

    const BufferVec3f& normalBuffer() const
    {
        return mNormalBuf;
    }

    const BufferVec4i& blendIndexBuffer() const
    {
        return mBlendIndexBuf;
    }

    const BufferVec4f& blendWeightBuffer() const
    {
        return mBlendWeightBuf;
    }

    const BufferU32& indexBuffer() const
    {
        return mIdxBuf;
    }

    u32 numBones() const
    {
        return mBoneBuf.count();
    }

    const MeshBone* bones() const
    {
        return mBoneBuf.ptr();
    }

    const MeshBone& bone(u32 i) const
    {
        RIO_ASSERT(i < numBones());
        return mBoneBuf.ptr()[i];
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
    BufferVec3f mPositionBuf;       // Position Buffer
    BufferVec2f mTexCoordBuf;       // Texture coordinate Buffer
    BufferVec3f mNormalBuf;         // Normal Buffer
    BufferVec4i mBlendIndexBuf;     // Blend Index Buffer
    BufferVec4f mBlendWeightBuf;    // Blend Weight Buffer

    BufferU32   mIdxBuf;            // Index buffer

    MeshBoneBuf mBoneBuf;           // Mesh reference of bone

    Vector3f    mScale;             // Mesh local scale
    Vector3f    mRotate;            // Mesh local rotation
    Vector3f    mTranslate;         // Mesh local translation

    u32         mMatIdx;            // Material index
};
static_assert(std::is_pod<Mesh>::value);
static_assert(sizeof(Mesh) == 0x60);

} } }

#endif // RIO_GFX_MDL_RES_MESH_DATA_H
