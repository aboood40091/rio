#ifndef RIO_GFX_MDL_MATERIAL_H
#define RIO_GFX_MDL_MATERIAL_H

#include <gfx/mdl/res/rio_MaterialData.h>
#include <gpu/rio_RenderState.h>
#include <gpu/rio_Shader.h>

#include <vector>

namespace rio { namespace mdl {

class Material;
class Mesh;
class Model;

class Texture
{
public:
    Texture()
        : mpTexture(nullptr)
        , mTextureSampler()
        , mVSLocation(0xFFFFFFFF)
        , mFSLocation(0xFFFFFFFF)
    {
    }

    Texture2D* texture() const { return mpTexture; }

    TextureSampler2D& textureSampler() { return mTextureSampler; }
    const TextureSampler2D& textureSampler() const { return mTextureSampler; }

    u32 vertexShaderLocation() const { return mVSLocation; }
    u32 fragmentShaderLocation() const { return mFSLocation; }

    void bind(u32 slot) const;

private:
    Texture2D*          mpTexture;          // 2D texture pointer.
    TextureSampler2D    mTextureSampler;    // 2D texture sampler object.
    u32                 mVSLocation;        // Vertex shader location.
    u32                 mFSLocation;        // Fragment shader location.

    friend class Material;
};

class UniformVar
{
public:
    res::UniformVar::Type type() const { return mType; }

    u32 vertexShaderLocation() const { return mVSLocation; }
    u32 fragmentShaderLocation() const { return mFSLocation; }

    const void* buffer() const { return mpBuf; }
    u32 bufferSize() const { return mBufSize; }

    void bind() const;

private:
    res::UniformVar::Type   mType;          // Uniform type.
    u32                     mVSLocation;    // Vertex shader location.
    u32                     mFSLocation;    // Fragment shader location.
    const void*             mpBuf;          // Uniform value buffer.
    u32                     mBufSize;       // Uniform value buffer size.

    friend class Material;
};

class Material
{
public:
    Material(const res::Material* res_material, Model* parent_mdl);
    ~Material();

    const res::Material& resMaterial() const
    {
        return mResMaterial;
    }

    Model& parentModel() const
    {
        return mParentModel;
    }

    Shader::ShaderMode shaderMode() const
    {
        return mShaderMode;
    }

    Shader* shader() const
    {
        return mShader;
    }

    const std::vector<Mesh*>& meshes() const
    {
        return mMeshes;
    }

    void bind() const;

private:
    void pushBackMesh_(Mesh* mesh);

private:
    const res::Material&    mResMaterial;       // Material resource.
    Model&                  mParentModel;       // Parent model.

    Shader::ShaderMode      mShaderMode;        // Shader mode.
    Shader*                 mShader;            // Shader pointer.

    Texture*                mTextures;          // List of textures.
    u32                     mNumTextures;       // Textures count.

    UniformVar*             mUniformVars;       // List of uniform variables.
    u32                     mNumUniformVars;    // Uniform variables count.

    UniformBlock*           mUniformBlocks;     // List of uniform blocks.
    u32                     mNumUniformBlocks;  // Uniform blocks count.

    std::vector<Mesh*>      mMeshes;            // List of meshes using this material.

    RenderState             mRenderState;       // Render state.

    friend class Model;
};

} }

#endif // RIO_GFX_MDL_MATERIAL_H
