#include <gfx/mdl/rio_Material.h>
#include <gfx/mdl/rio_Mesh.h>
#include <gfx/mdl/rio_Model.h>
#include <gpu/rio_Drawer.h>
#include <misc/rio_MemUtil.h>

namespace rio { namespace mdl {

Material::Material(const res::Material* res_material, Model* parent_mdl)
    : mResMaterial(*res_material)
    , mParentModel(*parent_mdl)
    , mTextures(nullptr)
    , mUniformVars(nullptr)
{
    RIO_ASSERT(parent_mdl && res_material);

    bool has_uniform_vars   = mResMaterial.numUniformVars()   > 0;
    bool has_uniform_blocks = mResMaterial.numUniformBlocks() > 0;

    RIO_ASSERT(!(has_uniform_vars && has_uniform_blocks));

    if (has_uniform_vars)
        mShaderMode = Shader::MODE_UNIFORM_REGISTER;

    else if (has_uniform_blocks)
        mShaderMode = Shader::MODE_UNIFORM_BLOCK;

    else
        mShaderMode = Shader::MODE_INVALID;

    // TODO: Shader cacher
    mShader = new Shader();
    mShader->load(mResMaterial.shaderName(), mShaderMode);

    mNumTextures = mResMaterial.numTextures();
    if (mNumTextures > 0)
    {
        mTextures = new Texture[mNumTextures];
        const res::TextureRef* const textures = mResMaterial.textures();

        for (u32 i = 0; i < mNumTextures; i++)
        {
            const char* const texture_name = textures[i].name();
            const char* const sampler_name = textures[i].samplerName();

            Texture& texture = mTextures[i];

            // TODO: Texture cacher
            texture.mpTexture = new Texture2D(texture_name);
            texture.mTextureSampler.linkTexture2D(texture.mpTexture);

            texture.mVSLocation = mShader->getVertexSamplerLocation(sampler_name);
            texture.mFSLocation = mShader->getFragmentSamplerLocation(sampler_name);

            textures[i].initTextureSampler(texture.mTextureSampler);
        }
    }

    mNumUniformVars = mResMaterial.numUniformVars();
    if (mNumUniformVars > 0)
    {
        mUniformVars = new UniformVar[mNumUniformVars];
        const res::UniformVar* const uniform_vars = mResMaterial.uniformVars();

        for (u32 i = 0; i < mNumUniformVars; i++)
        {
            UniformVar& uniform_var = mUniformVars[i];
            const res::UniformVar& res_uniform_var = uniform_vars[i];

            const char* const uniform_name = res_uniform_var.name();

            uniform_var.mType    = res_uniform_var.type();
            uniform_var.mpBuf    = res_uniform_var.values().ptr();
            uniform_var.mBufSize = res_uniform_var.values().size();

            uniform_var.mVSLocation = mShader->getVertexUniformLocation(uniform_name);
            uniform_var.mFSLocation = mShader->getFragmentUniformLocation(uniform_name);
        }
    }

    mNumUniformBlocks = mResMaterial.numUniformBlocks();
    if (mNumUniformBlocks > 0)
    {
        mUniformBlocks = (UniformBlock*)MemUtil::alloc(mNumUniformBlocks * sizeof(UniformBlock), 4);
        const res::UniformBlock* const uniform_blocks = mResMaterial.uniformBlocks();

        for (u32 i = 0; i < mNumUniformBlocks; i++)
        {
            const res::UniformBlock& res_uniform_block = uniform_blocks[i];

            u32 vs_index = mShader->getVertexUniformBlockIndex(res_uniform_block.name());
            u32 fs_index = mShader->getFragmentUniformBlockIndex(res_uniform_block.name());

            new (&mUniformBlocks[i]) UniformBlock(res_uniform_block.stage(), vs_index, fs_index);
            UniformBlock& uniform_block = mUniformBlocks[i];

            uniform_block.setData(MemUtil::alloc(res_uniform_block.size(), Drawer::cUniformBlockAlignment),
                                  res_uniform_block.size());

            const res::UniformVar* const uniform_vars = res_uniform_block.uniforms().ptr();
            const u32 num_uniform_vars = res_uniform_block.uniforms().count();

            for (u32 j = 0; j < num_uniform_vars; j++)
                uniform_block.setSubDataInvalidate(uniform_vars[i].values().ptr(),
                                                   uniform_vars[i].offset(),
                                                   uniform_vars[i].values().size());
        }
    }

    mResMaterial.initRenderState(mRenderState);
}

Material::~Material()
{
    delete mShader;

    if (mNumTextures > 0)
    {
        for (u32 i = 0; i < mNumTextures; i++)
            delete mTextures[i].mpTexture;

        delete[] mTextures;
    }

    if (mNumUniformVars > 0)
        delete[] mUniformVars;

    if (mNumUniformBlocks > 0)
    {
        for (u32 i = 0; i < mNumUniformBlocks; i++)
        {
            MemUtil::free((void*)mUniformBlocks[i].getData());
            mUniformBlocks[i].~UniformBlock();
        }

        MemUtil::free(mUniformBlocks);
    }
}

void Material::pushBackMesh_(Mesh* mesh)
{
    mMeshes.push_back(mesh);
}

void Material::bind() const
{
    mRenderState.apply();
    mShader->bind();

    for (u32 i = 0; i < mNumTextures; i++)
        mTextures[i].bind(i);

    for (u32 i = 0; i < mNumUniformVars; i++)
        mUniformVars[i].bind();

    for (u32 i = 0; i < mNumUniformBlocks; i++)
        mUniformBlocks[i].bind();
}

void Texture::bind(u32 slot) const
{
    mTextureSampler.tryBind(mVSLocation, mFSLocation, slot);
}

void UniformVar::bind() const
{
    switch (mType)
    {
    case res::UniformVar::TYPE_FLOAT:
        RIO_ASSERT(mBufSize == sizeof(f32));
        Shader::setUniform(*(f32*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_INT:
        RIO_ASSERT(mBufSize == sizeof(s32));
        Shader::setUniform(*(s32*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_UINT:
        RIO_ASSERT(mBufSize == sizeof(u32));
        Shader::setUniform(*(u32*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_VEC2:
        RIO_ASSERT(mBufSize == sizeof(BaseVec2f));
        Shader::setUniform(*(BaseVec2f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_IVEC2:
        RIO_ASSERT(mBufSize == sizeof(BaseVec2i));
        Shader::setUniform(*(BaseVec2i*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_UVEC2:
        RIO_ASSERT(mBufSize == sizeof(BaseVec2u));
        Shader::setUniform(*(BaseVec2u*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_VEC3:
        RIO_ASSERT(mBufSize == sizeof(BaseVec3f));
        Shader::setUniform(*(BaseVec3f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_IVEC3:
        RIO_ASSERT(mBufSize == sizeof(BaseVec3i));
        Shader::setUniform(*(BaseVec3i*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_UVEC3:
        RIO_ASSERT(mBufSize == sizeof(BaseVec3u));
        Shader::setUniform(*(BaseVec3u*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_VEC4:
        RIO_ASSERT(mBufSize == sizeof(BaseVec4f));
        Shader::setUniform(*(BaseVec4f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_IVEC4:
        RIO_ASSERT(mBufSize == sizeof(BaseVec4i));
        Shader::setUniform(*(BaseVec4i*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_UVEC4:
        RIO_ASSERT(mBufSize == sizeof(BaseVec4u));
        Shader::setUniform(*(BaseVec4u*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX2:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx22f));
        Shader::setUniform(*(BaseMtx22f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX32:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx23f));
        Shader::setUniform(*(BaseMtx23f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX42:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx24f));
        Shader::setUniform(*(BaseMtx24f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX23:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx32f));
        Shader::setUniform(*(BaseMtx32f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX3:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx33f));
        Shader::setUniform(*(BaseMtx33f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX43:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx34f));
        Shader::setUniform(*(BaseMtx34f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX24:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx42f));
        Shader::setUniform(*(BaseMtx42f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX34:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx43f));
        Shader::setUniform(*(BaseMtx43f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_MTX4:
        RIO_ASSERT(mBufSize == sizeof(BaseMtx44f));
        Shader::setUniform(*(BaseMtx44f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_VEC4_ARR:
        RIO_ASSERT(mBufSize % sizeof(BaseVec4f) == 0);
        Shader::setUniformArray(mBufSize / sizeof(BaseVec4f), (BaseVec4f*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_IVEC4_ARR:
        RIO_ASSERT(mBufSize % sizeof(BaseVec4i) == 0);
        Shader::setUniformArray(mBufSize / sizeof(BaseVec4i), (BaseVec4i*)mpBuf, mVSLocation, mFSLocation);
        break;
    case res::UniformVar::TYPE_UVEC4_ARR:
        RIO_ASSERT(mBufSize % sizeof(BaseVec4u) == 0);
        Shader::setUniformArray(mBufSize / sizeof(BaseVec4u), (BaseVec4u*)mpBuf, mVSLocation, mFSLocation);
        break;
    }
}

} }
