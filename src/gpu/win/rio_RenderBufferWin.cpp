#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gfx/rio_Graphics.h>
#include <gpu/rio_RenderBuffer.h>

#include <misc/gl/rio_GL.h>

namespace rio {

void RenderBuffer::bindColorClear(f32 r, f32 g, f32 b, f32 a)
{
    RIO_ASSERT(getRenderTargetColor() != nullptr);

    // Bind the render buffer
    rio::Graphics::setViewport(0, 0, mSize.x, mSize.y);
    rio::Graphics::setScissor(0, 0, mSize.x, mSize.y);
    bind_();

    // Clear using the given color
    RIO_GL_CALL(glClearColor(r, g, b, a));
    RIO_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    // Set correct scissor
    rio::Graphics::setScissor(mScissorPos.x, mScissorPos.y, mScissorSize.x, mScissorSize.y);
}

void RenderBuffer::bindDepthClear(f32 depth)
{
    RIO_ASSERT(getRenderTargetDepth() != nullptr);

    // Bind the render buffer
    rio::Graphics::setViewport(0, 0, mSize.x, mSize.y);
    rio::Graphics::setScissor(0, 0, mSize.x, mSize.y);
    bind_();

    // Clear
    RIO_GL_CALL(glDepthMask(GL_TRUE));
    RIO_GL_CALL(glClearDepth(depth));
    RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

    // Set correct scissor
    rio::Graphics::setScissor(mScissorPos.x, mScissorPos.y, mScissorSize.x, mScissorSize.y);
}

void RenderBuffer::bindStencilClear(u8 stencil)
{
    RIO_ASSERT(getRenderTargetDepth() != nullptr);

    // Bind the render buffer
    rio::Graphics::setViewport(0, 0, mSize.x, mSize.y);
    rio::Graphics::setScissor(0, 0, mSize.x, mSize.y);
    bind_();

    // Clear
    RIO_GL_CALL(glClearStencil(stencil));
    RIO_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));

    // Set correct scissor
    rio::Graphics::setScissor(mScissorPos.x, mScissorPos.y, mScissorSize.x, mScissorSize.y);
}

void RenderBuffer::bindDepthStencilClear(f32 depth, u8 stencil)
{
    RIO_ASSERT(getRenderTargetDepth() != nullptr);

    // Bind the render buffer
    rio::Graphics::setViewport(0, 0, mSize.x, mSize.y);
    rio::Graphics::setScissor(0, 0, mSize.x, mSize.y);
    bind_();

    // Clear
    RIO_GL_CALL(glDepthMask(GL_TRUE));
    RIO_GL_CALL(glClearDepth(depth));
    RIO_GL_CALL(glClearStencil(stencil));
    RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    // Set correct scissor
    rio::Graphics::setScissor(mScissorPos.x, mScissorPos.y, mScissorSize.x, mScissorSize.y);
}

}

#endif // RIO_IS_WIN
