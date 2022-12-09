#ifndef RIO_GFX_MDL_SKELETON_H
#define RIO_GFX_MDL_SKELETON_H

#include <gfx/mdl/res/rio_SkeletonData.h>
#include <math/rio_Matrix.h>

#include <vector>

namespace rio { namespace mdl {

class Skeleton;

class Bone
{
public:
    Bone(const res::Bone* p_res_bone);

    const rio::Matrix34f& getNodeTransform() const
    {
        return static_cast<const rio::Matrix34f&>(mResBone.nodeTransform());
    }

    Bone* parent() const
    {
        return mpParent;
    }

    const std::vector<Bone*>& children() const
    {
        return mChildren;
    }

private:
    const res::Bone& mResBone;

    Bone* mpParent;
    std::vector<Bone*> mChildren;

    friend class Skeleton;
};

class Skeleton
{
public:
    Skeleton(const res::Skeleton* p_res_skl);

    Bone* root() const
    {
        return mpRoot;
    }

    const std::vector<Bone>& bones() const
    {
        return mBone;
    }

    u32 getBoneIndex(const Bone& bone) const
    {
        const Bone* bones = mBone.data();
        intptr_t idx = intptr_t(&bone) - intptr_t(bones);
        RIO_ASSERT(0 <= idx && size_t(idx) < mBone.size());
        return idx;
    }

private:
    const res::Skeleton& mResSkeleton;

    Bone* mpRoot;
    std::vector<Bone> mBone;
};

} }

#endif // RIO_GFX_MDL_SKELETON_H
