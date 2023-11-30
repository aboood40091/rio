#include <gpu/rio_RenderTarget.h>

#if RIO_IS_CAFE
#include <misc/rio_MemUtil.h>

#include <gx2/mem.h>
#endif // RIO_IS_CAFE

namespace rio {

RenderTargetColor::RenderTargetColor()
{
#if RIO_IS_CAFE
    MemUtil::set(&mInnerBuffer, 0, sizeof(GX2ColorBuffer));
#elif RIO_IS_WIN
    mHandle = GL_NONE;
#endif
}

void RenderTargetColor::linkNativeTexture2D(NativeTexture2DHandle handle)
{
    mUpdateRegs = true;
    setMipLevel(0);
#if RIO_IS_CAFE
    mInnerBuffer.surface = handle->surface;
    mInnerBuffer.surface.use = GX2_SURFACE_USE_COLOR_BUFFER;
#elif RIO_IS_WIN
    mHandle = handle;
#endif
}

void RenderTargetColor::invalidateGPUCache() const
{
#if RIO_IS_CAFE
    if (mInnerBuffer.surface.image)
        GX2Invalidate(GX2_INVALIDATE_MODE_COLOR_BUFFER, mInnerBuffer.surface.image, mInnerBuffer.surface.imageSize);

    if (mInnerBuffer.surface.mipmaps)
        GX2Invalidate(GX2_INVALIDATE_MODE_COLOR_BUFFER, mInnerBuffer.surface.mipmaps, mInnerBuffer.surface.mipmapSize);
#endif // RIO_IS_CAFE
}

void RenderTargetColor::bind(u32 index) const
{
    if (mUpdateRegs)
    {
        initRegs_();
        mUpdateRegs = false;
    }

#if RIO_IS_CAFE
    GX2SetColorBuffer(&mInnerBuffer, static_cast<GX2RenderTarget>(GX2_RENDER_TARGET_0 + index));
#elif RIO_IS_WIN
    RIO_ASSERT(mHandle != GL_NONE);
    RIO_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, mHandle, mMipLevel));
#endif
}

void RenderTargetColor::initRegs_() const
{
#if RIO_IS_CAFE
    RIO_ASSERT(mMipLevel < mInnerBuffer.surface.mipLevels);
    mInnerBuffer.viewMip = mMipLevel;
    mInnerBuffer.viewFirstSlice = 0;
    mInnerBuffer.viewNumSlices = 1;
    GX2InitColorBufferRegs(&mInnerBuffer);
#endif // RIO_IS_CAFE
    mUpdateRegs = false;
}

RenderTargetDepth::RenderTargetDepth()
{
#if RIO_IS_CAFE
    MemUtil::set(&mInnerBuffer, 0, sizeof(GX2DepthBuffer));
    GX2InitDepthBufferHiZEnable(&mInnerBuffer, GX2_DISABLE);
#elif RIO_IS_WIN
    mHandle = GL_NONE;
    mHasStencil = false;
#endif
}

void RenderTargetDepth::linkNativeTexture2D(NativeTexture2DHandle handle)
{
    mUpdateRegs = true;
    setMipLevel(0);
#if RIO_IS_CAFE
    mInnerBuffer.surface = handle->surface;
    mInnerBuffer.surface.use = GX2_SURFACE_USE_DEPTH_BUFFER;
#elif RIO_IS_WIN
    mHandle = handle;
    mHasStencil = false;
#endif
}

void RenderTargetDepth::invalidateGPUCache() const
{
#if RIO_IS_CAFE
    if (mInnerBuffer.surface.image)
        GX2Invalidate(GX2_INVALIDATE_MODE_DEPTH_BUFFER, mInnerBuffer.surface.image, mInnerBuffer.surface.imageSize);

    if (mInnerBuffer.surface.mipmaps)
        GX2Invalidate(GX2_INVALIDATE_MODE_DEPTH_BUFFER, mInnerBuffer.surface.mipmaps, mInnerBuffer.surface.mipmapSize);
#endif // RIO_IS_CAFE
}

void RenderTargetDepth::bind() const
{
    if (mUpdateRegs)
    {
        initRegs_();
        mUpdateRegs = false;
    }

#if RIO_IS_CAFE
    GX2SetDepthBuffer(&mInnerBuffer);
#elif RIO_IS_WIN
    RIO_ASSERT(mHandle != GL_NONE);
    RIO_GL_CALL(glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        mHasStencil
            ? GL_DEPTH_STENCIL_ATTACHMENT
            : GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, mHandle, mMipLevel));
#endif
}

void RenderTargetDepth::initRegs_() const
{
#if RIO_IS_CAFE
    RIO_ASSERT(mMipLevel < mInnerBuffer.surface.mipLevels);
    mInnerBuffer.viewMip = mMipLevel;
    mInnerBuffer.viewFirstSlice = 0;
    mInnerBuffer.viewNumSlices = 1;
    mInnerBuffer.depthClear = 1.0f;
    mInnerBuffer.stencilClear = 0;
    GX2InitDepthBufferRegs(&mInnerBuffer);
#endif // RIO_IS_CAFE
    mUpdateRegs = false;
}

}
