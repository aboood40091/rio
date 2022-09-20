#ifndef RIO_GFX_MDL_RES_MATERIAL_DATA_H
#define RIO_GFX_MDL_RES_MATERIAL_DATA_H

#include <gfx/mdl/res/rio_ModelCommonData.h>
#include <gfx/rio_Color.h>
#include <gpu/rio_TextureSampler.h>
#include <gpu/rio_UniformBlock.h>
#include <math/rio_Vector.h>

namespace rio {

class RenderState;

}

namespace rio { namespace mdl { namespace res {

class UniformVar
{
    // Serializable class for shader uniform variables.

public:
    enum Type : u32
    {
        TYPE_FLOAT = 0, // These names match the glsl type.
        TYPE_INT,
        TYPE_UINT,
        TYPE_VEC2,
        TYPE_IVEC2,
        TYPE_UVEC2,
        TYPE_VEC3,
        TYPE_IVEC3,
        TYPE_UVEC3,
        TYPE_VEC4,
        TYPE_IVEC4,
        TYPE_UVEC4,
        TYPE_MTX2,
        TYPE_MTX32,
        TYPE_MTX42,
        TYPE_MTX23,
        TYPE_MTX3,
        TYPE_MTX43,
        TYPE_MTX24,
        TYPE_MTX34,
        TYPE_MTX4,
        TYPE_VEC4_ARR,
        TYPE_IVEC4_ARR,
        TYPE_UVEC4_ARR
    };

    const char* name() const
    {
        RIO_ASSERT(mName.ptr()[mName.count() - 1] == '\0');
        return mName.ptr();
    }

    Type type() const
    {
        return mType;
    }

    u32 offset() const
    {
        return mOffset;
    }

    const BufferU8& values() const
    {
        return mValuesBuf;
    }

private:
    FixedString mName;      // Uniform name. (MUST match with name in shader if variable is NOT part of a uniform block.)
    Type        mType;      // Uniform type. (MUST match with type in shader if variable is NOT part of a uniform block.)
    u32         mOffset;    // Uniform offset in uniform block data. (Only valid if variable IS part of a uniform block.)
    BufferU8    mValuesBuf; // Uniform default value buffer.
};
static_assert(std::is_pod<UniformVar>::value);
static_assert(sizeof(UniformVar) == 0x18);

class UniformBlock
{
    // Serializable class for shader uniform blocks with std140 layout.

public:
    const char* name() const
    {
        RIO_ASSERT(mName.ptr()[mName.count() - 1] == '\0');
        return mName.ptr();
    }

    ::rio::UniformBlock::ShaderStage stage() const
    {
        return mStage;
    }

    u32 size() const
    {
        return mSize;
    }

    const Buffer<UniformVar>& uniforms() const
    {
        return mUniforms;
    }

private:
    FixedString                         mName;      // Block name.
    ::rio::UniformBlock::ShaderStage    mStage;     // Block shader stage.
    u32                                 _unused0;   // Unused (for now).
    u32                                 mSize;      // Total block size.
    Buffer<UniformVar>                  mUniforms;  // List of variables.
};
static_assert(std::is_pod<UniformBlock>::value);
static_assert(sizeof(UniformBlock) == 0x1C);

class TextureRef
{
    // Serializable class for texture references and samplers.

public:
    const char* name() const
    {
        RIO_ASSERT(mName.ptr()[mName.count() - 1] == '\0');
        return mName.ptr();
    }

    const char* samplerName() const
    {
        RIO_ASSERT(mSamplerName.ptr()[mSamplerName.count() - 1] == '\0');
        return mSamplerName.ptr();
    }

    void initTextureSampler(TextureSampler2D& sampler) const
    {
        sampler.setFilter(mMagFilter, mMinFilter, mMipFilter, mMaxAniso);
        sampler.setWrap(mWrapX, mWrapY, mWrapZ);
        sampler.setBorderColor(mBorderColor.r, mBorderColor.g, mBorderColor.b, mBorderColor.a);
        sampler.setLOD(mMinLOD, mMaxLOD, mLODBias);
    }

private:
    FixedString         mName;          // Texture filename (without extension).
    FixedString         mSamplerName;   // Texture sampler name. (MUST match with name in shader.)

    // -------------------------           Sampler parameters           -------------------------

    TexXYFilterMode     mMagFilter;
    TexXYFilterMode     mMinFilter;
    TexMipFilterMode    mMipFilter;
    TexAnisoRatio       mMaxAniso;

    TexWrapMode         mWrapX;
    TexWrapMode         mWrapY;
    TexWrapMode         mWrapZ;

    Color4f             mBorderColor;

    f32                 mMinLOD;
    f32                 mMaxLOD;
    f32                 mLODBias;

    // ------------------------------------------------------------------------------------------
};
static_assert(std::is_pod<TextureRef>::value);
static_assert(sizeof(TextureRef) == 0x48);

class Material
{
public:
    const char* name() const
    {
        RIO_ASSERT(mName.ptr()[mName.count() - 1] == '\0');
        return mName.ptr();
    }

    const char* shaderName() const
    {
        RIO_ASSERT(mShaderName.ptr()[mShaderName.count() - 1] == '\0');
        return mShaderName.ptr();
    }

    bool isVisible() const
    {
        return mFlags & 1;
    }

    bool isTranslucent() const
    {
        return mRenderFlags & TRANSLUCENT;
    }

    u32 numTextures() const
    {
        return mTextures.count();
    }

    const TextureRef* textures() const
    {
        return mTextures.ptr();
    }

    const TextureRef& texture(u32 i) const
    {
        RIO_ASSERT(i < numTextures());
        return mTextures.ptr()[i];
    }

    u32 numUniformVars() const
    {
        return mUniformVars.count();
    }

    const UniformVar* uniformVars() const
    {
        return mUniformVars.ptr();
    }

    const UniformVar& uniformVar(u32 i) const
    {
        RIO_ASSERT(i < numUniformVars());
        return mUniformVars.ptr()[i];
    }

    u32 numUniformBlocks() const
    {
        return mUniformBlocks.count();
    }

    const UniformBlock* uniformBlocks() const
    {
        return mUniformBlocks.ptr();
    }

    const UniformBlock& uniformBlock(u32 i) const
    {
        RIO_ASSERT(i < numUniformBlocks());
        return mUniformBlocks.ptr()[i];
    }

    void initRenderState(RenderState& state) const;

private:
    enum RenderFlags : u16
    {
        DEPTH_TEST_ENABLE                = 0x0001,
        DEPTH_WRITE_ENABLE               = 0x0002,
        BLEND_ENABLE                     = 0x0004,
      //ALPHA_TEST_ENABLE                = 0x0008, // Deprecated.
        COLOR_MASK_R                     = 0x0010,
        COLOR_MASK_G                     = 0x0020,
        COLOR_MASK_B                     = 0x0040,
        COLOR_MASK_A                     = 0x0080,
        STENCIL_TEST_ENABLE              = 0x0100,
        POLYGON_OFFSET_ENABLE            = 0x0200,
        POLYGON_OFFSET_POINT_LINE_ENABLE = 0x0400,
        TRANSLUCENT                      = 0x0800
    };

    enum CompareFunc : u32
    {
        COMPARE_FUNC_NEVER = 0,
        COMPARE_FUNC_LESS,
        COMPARE_FUNC_EQUAL,
        COMPARE_FUNC_LEQUAL,
        COMPARE_FUNC_GREATER,
        COMPARE_FUNC_NOTEQUAL,
        COMPARE_FUNC_GEQUAL,
        COMPARE_FUNC_ALWAYS
    };

    enum CullingMode : u32
    {
        CULLING_MODE_FRONT = 0,
        CULLING_MODE_BACK,
        CULLING_MODE_NONE,
        CULLING_MODE_ALL
    };

    enum BlendFactor : u32
    {
        BLEND_MODE_ZERO = 0,
        BLEND_MODE_ONE,
        BLEND_MODE_SRC_COLOR,
        BLEND_MODE_ONE_MINUS_SRC_COLOR,
        BLEND_MODE_SRC_ALPHA,
        BLEND_MODE_ONE_MINUS_SRC_ALPHA,
        BLEND_MODE_DST_ALPHA,
        BLEND_MODE_ONE_MINUS_DST_ALPHA,
        BLEND_MODE_DST_COLOR,
        BLEND_MODE_ONE_MINUS_DST_COLOR,
        BLEND_MODE_SRC_ALPHA_SATURATE,
        BLEND_MODE_CONSTANT_COLOR,
        BLEND_MODE_ONE_MINUS_CONSTANT_COLOR,
        BLEND_MODE_CONSTANT_ALPHA,
        BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA
    };

    enum BlendEquation : u32
    {
        BLEND_FUNC_ADD = 0,
        BLEND_FUNC_SUB,
        BLEND_FUNC_MIN,
        BLEND_FUNC_MAX,
        BLEND_FUNC_REVERSE_SUB
    };

    enum StencilOp : u32
    {
        STENCIL_KEEP = 0,
        STENCIL_ZERO,
        STENCIL_REPLACE,
        STENCIL_INCR,
        STENCIL_DECR,
        STENCIL_INVERT,
        STENCIL_INCR_WRAP,
        STENCIL_DECR_WRAP
    };

    enum PolygonMode : u32
    {
        POLYGON_MODE_POINT = 0,
        POLYGON_MODE_LINE,
        POLYGON_MODE_FILL
    };

    FixedString             mName;                          // Material name.
    FixedString             mShaderName;                    // Shader filename (without extension).

    Buffer<TextureRef>      mTextures;                      // List of textures.
    Buffer<UniformVar>      mUniformVars;                   // List of uniform variables. (If and only if there are no uniform blocks.)
    Buffer<UniformBlock>    mUniformBlocks;                 // List of uniform blocks.

    u16                     mFlags;                         // General flags.
    u16                     mRenderFlags;                   // Render state flags.

    // ---------------------------------------------           Render state parameters            ---------------------------------------------

  //bool                    mIsVisible;                                     -> mFlags & 1
  //bool                    mIsTranslucent;                                 -> mRenderFlags & TRANSLUCENT

  //bool                    mDepthTestEnable;                               -> mRenderFlags & DEPTH_TEST_ENABLE
  //bool                    mDepthWriteEnable;                              -> mRenderFlags & DEPTH_WRITE_ENABLE
    CompareFunc             mDepthFunc;
    CullingMode             mCullingMode;
  //bool                    mBlendEnable;                                   -> mRenderFlags & BLEND_ENABLE
    BlendFactor             mBlendFactorSrcRGB;
    BlendFactor             mBlendFactorSrcA;
    BlendFactor             mBlendFactorDstRGB;
    BlendFactor             mBlendFactorDstA;
    BlendEquation           mBlendEquationRGB;
    BlendEquation           mBlendEquationA;
    Color4f                 mBlendConstantColor;
  //bool                    mAlphaTestEnable;               // Deprecated. (-> mRenderFlags & ALPHA_TEST_ENABLE)
    CompareFunc             mAlphaTestFunc;                 // ^^^
    f32                     mAlphaTestRef;                  // ^^^
  //bool                    mColorMaskR;                                    -> mRenderFlags & COLOR_MASK_R
  //bool                    mColorMaskG;                                    -> mRenderFlags & COLOR_MASK_G
  //bool                    mColorMaskB;                                    -> mRenderFlags & COLOR_MASK_B
  //bool                    mColorMaskA;                                    -> mRenderFlags & COLOR_MASK_A
  //bool                    mStencilTestEnable;                             -> mRenderFlags & STENCIL_TEST_ENABLE
    CompareFunc             mStencilTestFunc;
    s32                     mStencilTestRef;
    u32                     mStencilTestMask;
    StencilOp               mStencilOpFail;
    StencilOp               mStencilOpZFail;
    StencilOp               mStencilOpZPass;
    PolygonMode             mPolygonMode;
  //bool                    mPolygonOffsetEnable;                           -> mRenderFlags & POLYGON_OFFSET_ENABLE
  //bool                    mPolygonOffsetPointLineEnable;                  -> mRenderFlags & POLYGON_OFFSET_POINT_LINE_ENABLE

    // ----------------------------------------------------------------------------------------------------------------------------------------
};
static_assert(std::is_pod<Material>::value);
static_assert(sizeof(Material) == 0x80);

} } }

#endif // RIO_GFX_MDL_RES_MATERIAL_DATA_H
