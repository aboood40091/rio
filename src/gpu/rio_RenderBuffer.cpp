#include <gfx/rio_Graphics.h>
#include <gpu/rio_RenderBuffer.h>
#include <gpu/rio_RenderTarget.h>

namespace rio {

RenderBuffer::RenderBuffer()
    : mSize { 1, 1 }
    , mpColorTarget(nullptr)
    , mpDepthTarget(nullptr)
{
#if RIO_IS_WIN
    createHandle_();
#endif // RIO_IS_WIN
}

RenderBuffer::RenderBuffer(u32 w, u32 h)
    : mSize { s32(w), s32(h) }
    , mpColorTarget(nullptr)
    , mpDepthTarget(nullptr)
{
#if RIO_IS_WIN
    createHandle_();
#endif // RIO_IS_WIN
}

RenderBuffer::RenderBuffer(const rio::BaseVec2i& size)
    : mSize(size)
    , mpColorTarget(nullptr)
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
}

RenderBuffer::~RenderBuffer()
{
    if (mHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteFramebuffers(1, &mHandle));
        mHandle = GL_NONE;
    }
}

#endif // RIO_IS_WIN

void RenderBuffer::bind() const
{
    rio::Graphics::setViewport(0, 0, mSize.x, mSize.y);
    rio::Graphics::setScissor(0, 0, mSize.x, mSize.y);

#if RIO_IS_WIN
    RIO_ASSERT(mHandle != GL_NONE);
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));
#endif // RIO_IS_WIN

    if (mpColorTarget)
        mpColorTarget->bind();

    if (mpDepthTarget)
        mpDepthTarget->bind();
}

}
