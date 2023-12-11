#include <gfx/rio_Graphics.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_RenderBuffer.h>
#include <gpu/rio_RenderTarget.h>
#include <misc/rio_MemUtil.h>

#if RIO_IS_CAFE
#include <gx2/clear.h>
#include <gx2/event.h>
#endif // RIO_IS_CAFE

namespace rio {

RenderBuffer::RenderBuffer()
    : mSize { 1, 1 }
    , mScissorPos { 0, 0 }
    , mScissorSize { mSize }
    , mpColorTarget()
    , mpDepthTarget(nullptr)
{
#if RIO_IS_WIN
    createHandle_();
#endif // RIO_IS_WIN
}

RenderBuffer::RenderBuffer(u32 w, u32 h)
    : mSize { s32(w), s32(h) }
    , mScissorPos { 0, 0 }
    , mScissorSize { mSize }
    , mpColorTarget()
    , mpDepthTarget(nullptr)
{
#if RIO_IS_WIN
    createHandle_();
#endif // RIO_IS_WIN
}

RenderBuffer::RenderBuffer(const BaseVec2i& size)
    : mSize(size)
    , mScissorPos { 0, 0 }
    , mScissorSize { mSize }
    , mpColorTarget()
    , mpDepthTarget(nullptr)
{
#if RIO_IS_WIN
    createHandle_();
#endif // RIO_IS_WIN
}

#if RIO_IS_WIN

void RenderBuffer::createHandle_()
{
    RIO_GL_CALL(glGenFramebuffers(1, &mHandle));
    RIO_ASSERT(mHandle != GL_NONE);

    MemUtil::set(mDrawBuffers, 0, sizeof(mDrawBuffers));
}

RenderBuffer::~RenderBuffer()
{
    if (mHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteFramebuffers(1, &mHandle));
        mHandle = GL_NONE;
    }
}

void RenderBuffer::bindFBO_() const
{
    RIO_ASSERT(mHandle != GL_NONE);
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));
}

#endif // RIO_IS_WIN

void RenderBuffer::bind() const
{
    Graphics::setViewport(0, 0, mSize.x, mSize.y, 0.0f, 1.0f, mSize.y);
    Graphics::setScissor(mScissorPos.x, mScissorPos.y, mScissorSize.x, mScissorSize.y, mSize.y);

#if RIO_IS_WIN
    bindFBO_();
#endif // RIO_IS_WIN
    bindRenderTargetColor_();
    bindRenderTargetDepth_();
}

void RenderBuffer::bindRenderTargetColor_() const
{
    for (u32 i = 0; i < Graphics::RENDER_TARGET_MAX_NUM; i++)
    {
        if (mpColorTarget[i])
        {
            mpColorTarget[i]->bind(i);
#if RIO_IS_WIN
            mDrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
#endif // RIO_IS_WIN
        }
#if RIO_IS_WIN
        else
        {
            mDrawBuffers[i] = GL_NONE;
        }
#endif // RIO_IS_WIN
    }

#if RIO_IS_WIN
    RIO_GL_CALL(glDrawBuffers(Graphics::RENDER_TARGET_MAX_NUM, mDrawBuffers));
#endif // RIO_IS_WIN
}

void RenderBuffer::bindRenderTargetDepth_() const
{
    if (mpDepthTarget)
        mpDepthTarget->bind();
}

void RenderBuffer::clear(u32 color_target_index, u32 clear_flag, const Color4f& color, f32 depth, u8 stencil)
{
    const RenderTargetColor* p_color_target = getRenderTargetColor(color_target_index);
    const RenderTargetDepth* p_depth_target = color_target_index == 0 ? getRenderTargetDepth() : nullptr;

    clear_flag &= CLEAR_FLAG_COLOR_DEPTH_STENCIL;
    if (clear_flag & CLEAR_FLAG_COLOR && p_color_target == nullptr)
        clear_flag &= ~CLEAR_FLAG_COLOR;
    if (clear_flag & CLEAR_FLAG_DEPTH_STENCIL && p_depth_target == nullptr)
        clear_flag &= ~CLEAR_FLAG_DEPTH_STENCIL;

    if (clear_flag)
    {
#if RIO_IS_CAFE
        u32 depth_stencil_clear_flag = 0;
        if (clear_flag & CLEAR_FLAG_DEPTH)
        {
            depth_stencil_clear_flag |= GX2_CLEAR_FLAGS_DEPTH;
            p_depth_target->getNativeDepthBuffer().depthClear = depth;
        }
        if (clear_flag & CLEAR_FLAG_STENCIL)
        {
            depth_stencil_clear_flag |= GX2_CLEAR_FLAGS_STENCIL;
            p_depth_target->getNativeDepthBuffer().stencilClear = stencil;
        }

        if (clear_flag & CLEAR_FLAG_COLOR)
        {
            if (depth_stencil_clear_flag)
            {
                GX2ClearBuffersEx(
                    &p_color_target->getNativeColorBuffer(),
                    &p_depth_target->getNativeDepthBuffer(),
                    color.r, color.g, color.b, color.a,
                    depth, stencil, GX2ClearFlags(depth_stencil_clear_flag)
                );
            }
            else
            {
                GX2ClearColor(
                    &p_color_target->getNativeColorBuffer(),
                    color.r, color.g, color.b, color.a
                );
            }
        }
        else
        {
            RIO_ASSERT(depth_stencil_clear_flag);
            GX2ClearDepthStencilEx(
                &p_depth_target->getNativeDepthBuffer(),
                depth, stencil, GX2ClearFlags(depth_stencil_clear_flag)
            );
        }
#elif RIO_IS_WIN
        Graphics::setViewport(0, 0, mSize.x, mSize.y, 0.0f, 1.0f, mSize.y);
        Graphics::setScissor(0, 0, mSize.x, mSize.y, mSize.y);

        bindFBO_();

        if (clear_flag & CLEAR_FLAG_COLOR)
        {
            p_color_target->bind(color_target_index);
            RIO_GL_CALL(glDrawBuffer(GL_COLOR_ATTACHMENT0 + color_target_index));
        }

        if (clear_flag & CLEAR_FLAG_DEPTH_STENCIL)
            p_depth_target->bind();

        GLbitfield clear_mask = 0;

        if (clear_flag & CLEAR_FLAG_COLOR)
        {
            RIO_GL_CALL(glClearColor(color.r, color.g, color.b, color.a));
            clear_mask |= GL_COLOR_BUFFER_BIT;
        }

        if (clear_flag & CLEAR_FLAG_DEPTH)
        {
            RIO_GL_CALL(glDepthMask(GL_TRUE));
            RIO_GL_CALL(glClearDepth(depth));
            clear_mask |= GL_DEPTH_BUFFER_BIT;
        }

        if (clear_flag & CLEAR_FLAG_STENCIL)
        {
            RIO_GL_CALL(glStencilMask(0xFF));
            RIO_GL_CALL(glClearStencil(stencil));
            clear_mask |= GL_STENCIL_BUFFER_BIT;
        }

        RIO_ASSERT(clear_mask != 0);
        RIO_GL_CALL(glClear(clear_mask));
#endif
    }

    Window::instance()->makeContextCurrent();

    u32 width = Window::instance()->getWidth();
    u32 height = Window::instance()->getHeight();

    Graphics::setViewport(0, 0, width, height);
    Graphics::setScissor(0, 0, width, height);
}

bool RenderBuffer::read(
    u32 color_target_index, void* pixels
#if RIO_IS_WIN
    , u32 width
    , u32 height
    , const NativeTextureFormat& native_format
#endif // RIO_IS_WIN
)
{
    RIO_ASSERT(pixels);

    bool ret = true;

    const RenderTargetColor* p_color_target = getRenderTargetColor(color_target_index);
    if (p_color_target == nullptr)
        ret = false;

    else
    {
#if RIO_IS_CAFE
        GX2DrawDone();
#if 0
        const GX2Surface& src = p_color_target->getNativeColorBuffer().surface;
        GX2Surface dst;
        RIO_ASSERT(src.dim == GX2_SURFACE_DIM_TEXTURE_2D);
        dst.dim = GX2_SURFACE_DIM_TEXTURE_2D;
        dst.width = src.width;
        dst.height = src.height;
        RIO_ASSERT(src.depth <= 1);
        dst.depth = 1;
        dst.mipLevels = 1;
        dst.format = src.format;
        RIO_ASSERT(src.aa == GX2_AA_MODE1X);
        dst.aa = GX2_AA_MODE1X;
        dst.use = GX2_SURFACE_USE_TEXTURE;
        dst.tileMode = GX2_TILE_MODE_LINEAR_SPECIAL;
        dst.swizzle = 0;
        GX2CalcSurfaceSizeAndAlignment(&dst);
        dst.image = pixels;
        GX2CopySurface(&src, 0, 0, &dst, 0, 0);
#endif
#elif RIO_IS_WIN
        bindFBO_();
        p_color_target->bind(color_target_index);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + color_target_index);
        glReadPixels(0, 0, width, height, native_format.format, native_format.type, pixels);
#endif
    }

    Window::instance()->makeContextCurrent();
    return ret;
}

}
