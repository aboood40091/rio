#ifndef RIO_GFX_MDL_RES_SKELETON_DATA_H
#define RIO_GFX_MDL_RES_SKELETON_DATA_H

#include <gfx/mdl/res/rio_ModelCommonData.h>
#include <math/rio_Quat.h>
#include <math/rio_Vector.h>

namespace rio { namespace mdl { namespace res {

class Bone
{
public:
    s32 parentIndex() const
    {
        return mParentIdx;
    }

    u32 numChildren() const
    {
        return mChildrenIdx.count();
    }

    const s32* childrenIndices() const
    {
        return mChildrenIdx.ptr();
    }

    s32 childIndex(u32 i) const
    {
        RIO_ASSERT(i < numChildren());
        return mChildrenIdx.ptr()[i];
    }

    const rio::BaseMtx34f& nodeTransform() const
    {
        return mNodeTransform;
    }

private:
    FixedString mName;

    s32         mParentIdx;
    BufferS32   mChildrenIdx;

    rio::BaseMtx34f mNodeTransform;
};
static_assert(std::is_pod<Bone>::value);
static_assert(sizeof(Bone) == 0x44);

class Skeleton
{
public:
    s32 rootIndex() const
    {
        return mRootIdx;
    }

    u32 numBones() const
    {
        return mBones.count();
    }

    const Bone* bones() const
    {
        return mBones.ptr();
    }

    const Bone& bone(u32 i) const
    {
        RIO_ASSERT(i < numBones());
        return mBones.ptr()[i];
    }


private:
    s32             mRootIdx;
    Buffer<Bone>    mBones;
};
static_assert(std::is_pod<Skeleton>::value);
static_assert(sizeof(Skeleton) == 0xC);

class KeyFrameVec3
{
public:
    f32 frame() const
    {
        return mFrame;
    }

    const rio::Vector3f& value() const
    {
        return static_cast<const rio::Vector3f&>(mValue);
    }

private:
    f32             mFrame;
    rio::BaseVec3f  mValue;
};
static_assert(std::is_pod<KeyFrameVec3>::value);
static_assert(sizeof(KeyFrameVec3) == 0x10);

class KeyFrameQuat
{
public:
    f32 frame() const
    {
        return mFrame;
    }

    const rio::Quatf& value() const
    {
        return static_cast<const rio::Quatf&>(mValue);
    }

private:
    f32             mFrame;
    rio::BaseQuatf  mValue;
};
static_assert(std::is_pod<KeyFrameQuat>::value);
static_assert(sizeof(KeyFrameQuat) == 0x14);

class BoneAnim
{
private:
    u32 mSklBoneIndex;

    Buffer<KeyFrameVec3>    mScalingKey;
    Buffer<KeyFrameQuat>    mRotationKey;
    Buffer<KeyFrameVec3>    mPositionKey;
};
static_assert(std::is_pod<BoneAnim>::value);
static_assert(sizeof(BoneAnim) == 0x1C);

class SkeletalAnimation
{
public:
    const char* name() const
    {
        RIO_ASSERT(mName.ptr()[mName.count() - 1] == '\0');
        return mName.ptr();
    }

private:
    FixedString mName;

    f32 mFPS;
    f32 mDuration;

    Buffer<BoneAnim> mBoneAnims;
};
static_assert(std::is_pod<SkeletalAnimation>::value);
static_assert(sizeof(SkeletalAnimation) == 0x18);

} } }

#endif // RIO_GFX_MDL_RES_SKELETON_DATA_H
