#ifndef RIO_GPU_RENDER_BUFFER_H
#define RIO_GPU_RENDER_BUFFER_H

#include <math/rio_Vector.h>

namespace rio {

class RenderTargetColor;
class RenderTargetDepth;

class RenderBuffer
{
public:
    RenderBuffer();
    RenderBuffer(u32 w, u32 h);
    RenderBuffer(const rio::BaseVec2i& size);
#if RIO_IS_WIN
    ~RenderBuffer();
#endif // RIO_IS_WIN

private:
    RenderBuffer(const RenderBuffer&);
    RenderBuffer& operator=(const RenderBuffer&);

public:
    RenderTargetColor* getRenderTargetColor() const
    {
        return mpColorTarget;
    }

    RenderTargetDepth* getRenderTargetDepth() const
    {
        return mpDepthTarget;
    }

    void setRenderTargetColor(RenderTargetColor* target)
    {
        mpColorTarget = target;
    }

    void setRenderTargetDepth(RenderTargetDepth* target)
    {
        mpDepthTarget = target;
    }

    void setRenderTargetColorNull() { setRenderTargetColor(nullptr); }
    void setRenderTargetDepthNull() { setRenderTargetDepth(nullptr); }

    const rio::BaseVec2i& getSize() const
    {
        return mSize;
    }

    void setSize(u32 w, u32 h)
    {
        mSize.x = w;
        mSize.y = h;
        resetScissor();
    }

    void setSize(const rio::BaseVec2i& size)
    {
        mSize = size;
        resetScissor();
    }

    void setScissor(s32 x, s32 y, u32 w, u32 h)
    {
        mScissorPos.x = x;
        mScissorPos.y = y;
        mScissorSize.x = w;
        mScissorSize.y = h;
    }

    void setScissor(const rio::BaseVec2i& pos, const rio::BaseVec2i& size)
    {
        mScissorPos = pos;
        mScissorSize = size;
    }

    void resetScissor()
    {
        mScissorPos.x = 0;
        mScissorPos.y = 0;
        mScissorSize = mSize;
    }

    void bind() const;
    void bindColorClear(f32 r, f32 g, f32 b, f32 a = 1.0f);
    void bindDepthClear(f32 depth = 1.0f);
    void bindStencilClear(u8 stencil = 0);
    void bindDepthStencilClear(f32 depth = 1.0f, u8 stencil = 0);

private:
    void bind_() const;

private:
    rio::BaseVec2i      mSize;
    rio::BaseVec2i      mScissorPos;
    rio::BaseVec2i      mScissorSize;
    RenderTargetColor*  mpColorTarget;
    RenderTargetDepth*  mpDepthTarget;
#if RIO_IS_WIN
    u32                 mHandle;

    void createHandle_();
#endif // RIO_IS_WIN
};

}

#endif // RIO_GPU_RENDER_BUFFER_H
