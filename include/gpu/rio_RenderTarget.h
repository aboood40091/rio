#ifndef RIO_GPU_RENDER_TARGET_H
#define RIO_GPU_RENDER_TARGET_H

#include <gpu/rio_Texture.h>

namespace rio {

class RenderTarget
{
public:
    static const u32 cMipLevelMax = 14;

public:
    RenderTarget()
        : mUpdateRegs(true)
        , mMipLevel(0)
    {
    }

    u32 getMipLevel() const
    {
        return mMipLevel;
    }

    void setMipLevel(u32 mip_level)
    {
        if (mMipLevel != mip_level)
        {
            RIO_ASSERT(mip_level < cMipLevelMax);
            mMipLevel = mip_level;
            mUpdateRegs = true;
        }
    }

    virtual void linkNativeTexture2D(NativeTexture2DHandle handle) = 0;
    virtual void invalidateGPUCache() const = 0;
    virtual void bind() const = 0;

protected:
    mutable bool    mUpdateRegs;
    u32             mMipLevel;
};

class RenderTargetColor : public RenderTarget
{
public:
    RenderTargetColor();

    void linkTexture2D(const Texture2D& texture)
    {
        RIO_ASSERT(TextureFormatUtil::isUsableAsRenderTargetColor(texture.getTextureFormat()));
        linkNativeTexture2D(texture.getNativeTextureHandle());
    }

    void linkNativeTexture2D(NativeTexture2DHandle handle) override;
    void invalidateGPUCache() const override;
    void bind() const override
    {
        bind(0);
    }

    void bind(u32 index) const;

#if RIO_IS_CAFE
    GX2ColorBuffer& getNativeColorBuffer() const
    {
        return mInnerBuffer;
    }
#endif // RIO_IS_CAFE

private:
    void initRegs_() const;

protected:
#if RIO_IS_CAFE
    mutable GX2ColorBuffer  mInnerBuffer;
#elif RIO_IS_WIN
    u32                     mHandle;
#endif
};

class RenderTargetDepth : public RenderTarget
{
public:
    RenderTargetDepth();

    void linkTexture2D(const Texture2D& texture)
    {
        RIO_ASSERT(TextureFormatUtil::isUsableAsRenderTargetDepth(texture.getTextureFormat()));
        linkNativeTexture2D(texture.getNativeTextureHandle(), TextureFormatUtil::hasStencil(texture.getTextureFormat()));
    }

    void linkNativeTexture2D(NativeTexture2DHandle handle) override;
    void linkNativeTexture2D(NativeTexture2DHandle handle, bool has_stencil)
    {
        linkNativeTexture2D(handle);
#if RIO_IS_WIN
        mHasStencil = has_stencil;
#endif // RIO_IS_WIN
    }

    void invalidateGPUCache() const override;
    void bind() const override;

#if RIO_IS_CAFE
    GX2DepthBuffer& getNativeDepthBuffer() const
    {
        return mInnerBuffer;
    }
#endif // RIO_IS_CAFE

private:
    void initRegs_() const;

protected:
#if RIO_IS_CAFE
    mutable GX2DepthBuffer  mInnerBuffer;
#elif RIO_IS_WIN
    u32                     mHandle;
    bool                    mHasStencil;
#endif
};

}

#endif // RIO_GPU_RENDER_TARGET_H
