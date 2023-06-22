#ifndef RIO_GPU_SAMPLER_H
#define RIO_GPU_SAMPLER_H

#include <gfx/rio_Graphics.h>
#include <gpu/rio_Texture.h>

#if RIO_IS_CAFE
#include <gx2/sampler.h>
#elif RIO_IS_WIN
#include <misc/gl/rio_GL.h>
#endif

namespace rio {

enum TexXYFilterMode : u32
{
    TEX_XY_FILTER_MODE_POINT  = 0,
    TEX_XY_FILTER_MODE_LINEAR = 1
};

enum TexMipFilterMode : u32
{
    TEX_MIP_FILTER_MODE_NONE   = 0,
    TEX_MIP_FILTER_MODE_POINT  = 1,
    TEX_MIP_FILTER_MODE_LINEAR = 2
};

enum TexAnisoRatio : u32
{
    TEX_ANISO_1_TO_1  = 0,
    TEX_ANISO_2_TO_1  = 1,
    TEX_ANISO_4_TO_1  = 2,
    TEX_ANISO_8_TO_1  = 3,
    TEX_ANISO_16_TO_1 = 4
};

enum TexWrapMode : u32
{
    TEX_WRAP_MODE_REPEAT                  = 0,
    TEX_WRAP_MODE_MIRROR                  = 1,
    TEX_WRAP_MODE_CLAMP                   = 2,
    TEX_WRAP_MODE_MIRROR_ONCE             = 3,
    TEX_WRAP_MODE_CLAMP_HALF_BORDER       = 4,
    TEX_WRAP_MODE_MIRROR_ONCE_HALF_BORDER = 5,
    TEX_WRAP_MODE_CLAMP_BORDER            = 6,
    TEX_WRAP_MODE_MIRROR_ONCE_BORDER      = 7
};

#if RIO_IS_CAFE
typedef GX2Sampler NativeSampler2D;
#elif RIO_IS_WIN
typedef GLuint NativeSampler2D;
#endif

class TextureSampler2D
{
public:
    TextureSampler2D();
    ~TextureSampler2D();

private:
    TextureSampler2D(const TextureSampler2D&);
    TextureSampler2D& operator=(const TextureSampler2D&);

public:
    void linkTexture2D(const Texture2D* texture);
    void linkNativeTexture2D(NativeTexture2DHandle handle);

    void setMagFilter(TexXYFilterMode mag_filter);
    void setMinFilter(TexXYFilterMode min_filter);
    void setMipFilter(TexMipFilterMode mip_filter);
    void setMaxAnisoRatio(TexAnisoRatio max_aniso);

    void setFilter(TexXYFilterMode  mag_filter,
                   TexXYFilterMode  min_filter,
                   TexMipFilterMode mip_filter,
                   TexAnisoRatio    max_aniso);

    void setWrapX(TexWrapMode wrap_x);
    void setWrapY(TexWrapMode wrap_y);
    void setWrapZ(TexWrapMode wrap_z);

    void setWrap(TexWrapMode wrap_x,
                 TexWrapMode wrap_y,
                 TexWrapMode wrap_z);

    void setBorderColor(f32 r, f32 g, f32 b, f32 a);

    void setLOD(f32 min_lod, f32 max_lod, f32 lod_bias);

    void setDepthCompareEnable(bool enable);
    void setDepthCompareFunc(Graphics::CompareFunc func);

    void update() const;

    bool isTextureAvailable() const
    {
        return mTexture2DHandle;
    }

    bool isBindable() const
    {
        return isTextureAvailable();
    }

    // Bind the sampler for the vertex shader
    void bindVS(u32 location, u32 slot) const;
    // Bind the sampler for the fragment shader
    void bindFS(u32 location, u32 slot) const;
    // Bind the sampler for both the vertex and fragment shaders
    void bind(u32 vs_location, u32 fs_location, u32 slot) const;

    bool tryBindVS(u32 location, u32 slot) const
    {
        if (location == 0xFFFFFFFF || !isBindable())
            return false;

        bindVS(location, slot);
        return true;
    }

    bool tryBindFS(u32 location, u32 slot) const
    {
        if (location == 0xFFFFFFFF || !isBindable())
            return false;

        bindFS(location, slot);
        return true;
    }

    bool tryBind(u32 vs_location, u32 fs_location, u32 slot) const
    {
        return tryBindVS(vs_location, slot) || tryBindFS(fs_location, slot);
    }

private:
    void init_();

    void updateFilter_() const;
    void updateWrap_() const;
    void updateBorderColor_() const;
    void updateLOD_() const;
    void updateDepthComp_() const;

private:
    mutable u8              mFlags;
    mutable bool            mHasBorder;
    bool                    mDepthCompareEnable;
    TexXYFilterMode         mMagFilter;
    TexXYFilterMode         mMinFilter;
    TexMipFilterMode        mMipFilter;
    TexAnisoRatio           mMaxAniso;
    TexWrapMode             mWrapX;
    TexWrapMode             mWrapY;
    TexWrapMode             mWrapZ;
    f32                     mBorderColor[4];
    f32                     mMinLOD;
    f32                     mMaxLOD;
    f32                     mLODBias;
    Graphics::CompareFunc   mDepthCompareFunc;
    mutable NativeSampler2D mSamplerInner;

    NativeTexture2DHandle   mTexture2DHandle;
};

inline void TextureSampler2D::init_()
{
    mFlags = 0xFF;
    mTexture2DHandle = RIO_NATIVE_TEXTURE_2D_HANDLE_NULL;

    mMagFilter = TEX_XY_FILTER_MODE_LINEAR;
    mMinFilter = TEX_XY_FILTER_MODE_LINEAR;
    mMipFilter = TEX_MIP_FILTER_MODE_LINEAR;
    mMaxAniso = TEX_ANISO_1_TO_1;

    mWrapX = TEX_WRAP_MODE_CLAMP;
    mWrapY = TEX_WRAP_MODE_CLAMP;
    mWrapZ = TEX_WRAP_MODE_CLAMP;

    mBorderColor[0] = 0.0f;
    mBorderColor[1] = 0.0f;
    mBorderColor[2] = 0.0f;
    mBorderColor[3] = 1.0f;
    mHasBorder = false;

    mMinLOD = 0.0f;
    mMaxLOD = 14.0f;
    mLODBias = 0.0f;

    mDepthCompareFunc = Graphics::COMPARE_FUNC_NEVER;
    mDepthCompareEnable = false;
}

inline void TextureSampler2D::linkTexture2D(const Texture2D* texture)
{
    RIO_ASSERT(texture);
    linkNativeTexture2D(texture->getNativeTextureHandle());
}

inline void TextureSampler2D::linkNativeTexture2D(NativeTexture2DHandle handle)
{
    RIO_ASSERT(handle);
    mTexture2DHandle = handle;
}

inline void TextureSampler2D::setMagFilter(TexXYFilterMode mag_filter)
{
    RIO_ASSERT(mag_filter == 0 || mag_filter == 1);
    mMagFilter = mag_filter;
    mFlags |= 1 << 0;
}

inline void TextureSampler2D::setMinFilter(TexXYFilterMode min_filter)
{
    RIO_ASSERT(min_filter == 0 || min_filter == 1);
    mMinFilter = min_filter;
    mFlags |= 1 << 0;
}

inline void TextureSampler2D::setMipFilter(TexMipFilterMode mip_filter)
{
    RIO_ASSERT(0 <= mip_filter && mip_filter <= 2);
    mMipFilter = mip_filter;
    mFlags |= 1 << 0;
}

inline void TextureSampler2D::setMaxAnisoRatio(TexAnisoRatio max_aniso)
{
    RIO_ASSERT(0 <= max_aniso && max_aniso <= 4);
    mMaxAniso = max_aniso;
    mFlags |= 1 << 0;
}

inline void TextureSampler2D::setFilter(TexXYFilterMode  mag_filter,
               TexXYFilterMode  min_filter,
               TexMipFilterMode mip_filter,
               TexAnisoRatio    max_aniso)
{
    RIO_ASSERT(mag_filter == 0 || mag_filter == 1);
    mMagFilter = mag_filter;

    RIO_ASSERT(min_filter == 0 || min_filter == 1);
    mMinFilter = min_filter;

    RIO_ASSERT(0 <= mip_filter && mip_filter <= 2);
    mMipFilter = mip_filter;

    RIO_ASSERT(0 <= max_aniso && max_aniso <= 4);
    mMaxAniso = max_aniso;

    mFlags |= 1 << 0;
}

inline void TextureSampler2D::setWrapX(TexWrapMode wrap_x)
{
    RIO_ASSERT(0 <= wrap_x && wrap_x <= 7);
    mWrapX = wrap_x;
    mFlags |= 1 << 1;
}

inline void TextureSampler2D::setWrapY(TexWrapMode wrap_y)
{
    RIO_ASSERT(0 <= wrap_y && wrap_y <= 7);
    mWrapY = wrap_y;
    mFlags |= 1 << 1;
}

inline void TextureSampler2D::setWrapZ(TexWrapMode wrap_z)
{
    RIO_ASSERT(0 <= wrap_z && wrap_z <= 7);
    mWrapZ = wrap_z;
    mFlags |= 1 << 1;
}

inline void TextureSampler2D::setWrap(TexWrapMode wrap_x,
             TexWrapMode wrap_y,
             TexWrapMode wrap_z)
{
    RIO_ASSERT(0 <= wrap_x && wrap_x <= 7);
    mWrapX = wrap_x;

    RIO_ASSERT(0 <= wrap_y && wrap_y <= 7);
    mWrapY = wrap_y;

    RIO_ASSERT(0 <= wrap_z && wrap_z <= 7);
    mWrapZ = wrap_z;

    mFlags |= 1 << 1;
}

inline void TextureSampler2D::setBorderColor(f32 r, f32 g, f32 b, f32 a)
{
    mBorderColor[0] = r;
    mBorderColor[1] = g;
    mBorderColor[2] = b;
    mBorderColor[3] = a;

    mFlags |= 1 << 2;
}

inline void TextureSampler2D::setLOD(f32 min_lod, f32 max_lod, f32 lod_bias)
{
    mMinLOD = min_lod;
    mMaxLOD = max_lod;
    mLODBias = lod_bias;

    mFlags |= 1 << 3;
}

inline void TextureSampler2D::setDepthCompareEnable(bool enable)
{
    mDepthCompareEnable = enable;

    mFlags |= 1 << 4;
}

inline void TextureSampler2D::setDepthCompareFunc(Graphics::CompareFunc func)
{
    mDepthCompareFunc = func;

    mFlags |= 1 << 4;
}

inline void TextureSampler2D::update() const
{
    if (mFlags)
    {
        if (mFlags >> 0 & 1)
            updateFilter_();

        if (mFlags >> 1 & 1)
            updateWrap_();

        if (mFlags >> 2 & 1)
            updateBorderColor_();

        if (mFlags >> 3 & 1)
            updateLOD_();

        if (mFlags >> 3 & 1)
            updateDepthComp_();

        mFlags = 0;
    }
}

#if RIO_IS_WIN

inline void TextureSampler2D::bindVS(u32 location, u32 slot) const
{
    return bind(location, location, slot);
}

inline void TextureSampler2D::bindFS(u32 location, u32 slot) const
{
    return bind(location, location, slot);
}

#endif // RIO_IS_WIN

}

#endif // RIO_GPU_SAMPLER_H
