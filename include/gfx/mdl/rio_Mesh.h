#ifndef RIO_GFX_MDL_MESH_H
#define RIO_GFX_MDL_MESH_H

#include <gfx/mdl/res/rio_MeshData.h>
#include <gpu/rio_VertexArray.h>
#include <math/rio_Matrix.h>

namespace rio { namespace mdl {

class Bone;
class Material;
class Model;
class Skeleton;

namespace res {

class SkeletalAnimation;

}

class Mesh
{
public:
    enum AttributeLocation
    {
        ATTRIB_POSITION = 0,
        ATTRIB_TEX_COORD,
        ATTRIB_NORMAL,
        ATTRIB_BLEND_INDEX,
        ATTRIB_BLEND_WEIGHT
    };

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

    rio::BaseMtx34f* boneTransform() const
    {
        return mBoneTransform;
    }

    s32 getMeshBoneIndex(const Skeleton& skeleton, const Bone& bone);

    void calcBoneBaseTransform(const Skeleton& skeleton);
    void applyAnim(f32 frame, const res::SkeletalAnimation& skl_anim, const Skeleton& skeleton, const Bone& bone, const rio::Matrix34f& parent_transform);

    void draw() const;

private:
    void calcBoneBaseTransform_(const Skeleton& skeleton, const Bone& bone, const rio::Matrix34f& parent_transform);

    void setMaterial_(Material* material);
    void calcLocalMtx_();
    void calcWorldMtx_(const Matrix34f& mdl_world_mtx);

private:
    const res::Mesh&    mResMesh;           // Mesh resource.
    Model&              mParentModel;       // Parent Model.

    Material*           mMaterial;          // Material to use.

    Matrix34f           mLocalMtx;          // Local transformation matrix.
    Matrix34f           mWorldMtx;          // World transformation matrix. (Model x Local)

    u32                 mVtxNum;            // Vertices count.

    const u32*          mIdxBuf;            // Index buffer.
    u32                 mIdxNum;            // Indices count.

    struct Attrib
    {
        Attrib(s32 buffer_index)
            : vtx_stream()
            , vtx_buf(buffer_index)
        {
        }

        VertexStream vtx_stream;
        VertexBuffer vtx_buf;
    };

    Attrib              mPositionAttrib;    // Position vertex attribute.
    Attrib              mTexCoordAttrib;    // Texture coordinates vertex attribute.
    Attrib              mNormalAttrib;      // Normal vertex attribute.
    Attrib              mBlendIndexAttrib;  // Blend index vertex attribute.
    Attrib              mBlendWeightAttrib; // Blend weight vertex attribute.
    VertexArray         mVAO;               // Vertex array object.

    rio::Matrix34f*     mBoneTransform;

    friend class Model;
};

} }

#endif // RIO_GFX_MDL_MESH_H
