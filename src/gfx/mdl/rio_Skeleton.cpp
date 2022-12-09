#include <gfx/mdl/rio_Skeleton.h>

namespace rio { namespace mdl {

Bone::Bone(const res::Bone* p_res_bone)
    : mResBone(*p_res_bone)
    , mpParent(nullptr)
{
}

Skeleton::Skeleton(const res::Skeleton* p_res_skl)
    : mResSkeleton(*p_res_skl)
    , mpRoot(nullptr)
{
    const auto bone_num = mResSkeleton.numBones();
    if (bone_num > 0)
    {
        mBone.reserve(bone_num);
        const res::Bone* bones = mResSkeleton.bones();

        for (u32 i = 0; i < bone_num; i++)
            mBone.emplace_back(&(bones[i]));

        const auto root_index = mResSkeleton.rootIndex();
        RIO_ASSERT(root_index >= 0);
        mpRoot = &(mBone[root_index]);

        for (Bone& bone : mBone)
        {
            const res::Bone& res_bone = bone.mResBone;
            const auto parent_index = res_bone.parentIndex();

            bone.mpParent = parent_index >= 0 ? &(mBone[parent_index]) : nullptr;

            const auto child_num = res_bone.numChildren();
            if (child_num > 0)
            {
                mBone.reserve(bone_num);
                const s32* child_indices = res_bone.childrenIndices();

                for (u32 i = 0; i < child_num; i++)
                {
                    const auto child_index = child_indices[i];
                    RIO_ASSERT(child_index >= 0);
                    bone.mChildren.push_back(&(mBone[child_index]));
                }
            }
        }
    }
}

} }