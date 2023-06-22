#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_TextureSampler.h>

#include <gx2/shaders.h>

extern "C" {

void GX2SetVertexSamplerBorderColor(
    u32 location,
    f32 r, f32 g, f32 b, f32 a
);

void GX2SetPixelSamplerBorderColor(
    u32 location,
    f32 r, f32 g, f32 b, f32 a
);

}

namespace rio {

TextureSampler2D::TextureSampler2D()
{
    init_();

    GX2InitSampler(&mSamplerInner,
                   GX2_TEX_CLAMP_MODE_CLAMP,
                   GX2_TEX_XY_FILTER_MODE_LINEAR);
}

TextureSampler2D::~TextureSampler2D()
{
}

void TextureSampler2D::updateFilter_() const
{
    GX2InitSamplerXYFilter(&mSamplerInner,
                           (GX2TexXYFilterMode)mMagFilter,
                           (GX2TexXYFilterMode)mMinFilter,
                           (GX2TexAnisoRatio)mMaxAniso);

    GX2InitSamplerZMFilter(&mSamplerInner,
                           GX2_TEX_Z_FILTER_MODE_NONE,
                           (GX2TexMipFilterMode)mMipFilter);
}

void TextureSampler2D::updateWrap_() const
{
    GX2InitSamplerClamping(&mSamplerInner,
                           (GX2TexClampMode)mWrapX,
                           (GX2TexClampMode)mWrapY,
                           (GX2TexClampMode)mWrapZ);
}

void TextureSampler2D::updateBorderColor_() const
{
    if (mBorderColor[0] == 0.0f &&
        mBorderColor[1] == 0.0f &&
        mBorderColor[2] == 0.0f &&
        mBorderColor[3] == 1.0f)
    {
        GX2InitSamplerBorderType(&mSamplerInner, GX2_TEX_BORDER_TYPE_BLACK);
        mHasBorder = false;
    }
    else if (mBorderColor[0] == 1.0f &&
             mBorderColor[1] == 1.0f &&
             mBorderColor[2] == 1.0f &&
             mBorderColor[3] == 1.0f)
    {
        GX2InitSamplerBorderType(&mSamplerInner, GX2_TEX_BORDER_TYPE_WHITE);
        mHasBorder = false;
    }
    else if (mBorderColor[0] == 0.0f &&
             mBorderColor[1] == 0.0f &&
             mBorderColor[2] == 0.0f &&
             mBorderColor[3] == 0.0f)
    {
        GX2InitSamplerBorderType(&mSamplerInner, GX2_TEX_BORDER_TYPE_TRANSPARENT_BLACK);
        mHasBorder = false;
    }
    else
    {
        GX2InitSamplerBorderType(&mSamplerInner, GX2_TEX_BORDER_TYPE_VARIABLE);
        mHasBorder = true;
    }
}

void TextureSampler2D::updateLOD_() const
{
    GX2InitSamplerLOD(&mSamplerInner, mMinLOD, mMaxLOD, mLODBias);
}

void TextureSampler2D::updateDepthComp_() const
{
    GX2InitSamplerDepthCompare(&mSamplerInner, (GX2CompareFunction)mDepthCompareFunc);
}

void TextureSampler2D::bindVS(u32 location, u32) const
{
    RIO_ASSERT(location != 0xFFFFFFFF);
    RIO_ASSERT(isBindable());

    update();

    if (mHasBorder)
        GX2SetVertexSamplerBorderColor(location, mBorderColor[0],
                                                 mBorderColor[1],
                                                 mBorderColor[2],
                                                 mBorderColor[3]);

    GX2SetVertexTexture(mTexture2DHandle, location);
    GX2SetVertexSampler(&mSamplerInner, location);
}

void TextureSampler2D::bindFS(u32 location, u32) const
{
    RIO_ASSERT(location != 0xFFFFFFFF);
    RIO_ASSERT(isBindable());

    update();

    if (mHasBorder)
        GX2SetPixelSamplerBorderColor(location, mBorderColor[0],
                                                mBorderColor[1],
                                                mBorderColor[2],
                                                mBorderColor[3]);

    GX2SetPixelTexture(mTexture2DHandle, location);
    GX2SetPixelSampler(&mSamplerInner, location);
}

void TextureSampler2D::bind(u32 vs_location, u32 fs_location, u32) const
{
    RIO_ASSERT(vs_location != 0xFFFFFFFF && fs_location != 0xFFFFFFFF);
    RIO_ASSERT(isBindable());

    update();

    if (mHasBorder)
        GX2SetVertexSamplerBorderColor(vs_location, mBorderColor[0],
                                                    mBorderColor[1],
                                                    mBorderColor[2],
                                                    mBorderColor[3]);

    GX2SetVertexTexture(mTexture2DHandle, vs_location);
    GX2SetVertexSampler(&mSamplerInner, vs_location);

    if (mHasBorder)
        GX2SetPixelSamplerBorderColor(fs_location, mBorderColor[0],
                                                   mBorderColor[1],
                                                   mBorderColor[2],
                                                   mBorderColor[3]);

    GX2SetPixelTexture(mTexture2DHandle, fs_location);
    GX2SetPixelSampler(&mSamplerInner, fs_location);
}

}

#endif // RIO_IS_CAFE
