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
    }

    void setSize(const rio::BaseVec2i& size)
    {
        mSize = size;
    }

    void bind() const;

private:
    rio::BaseVec2i      mSize;
    RenderTargetColor*  mpColorTarget;
    RenderTargetDepth*  mpDepthTarget;
#if RIO_IS_WIN
    u32                 mHandle;

    void createHandle_();
#endif // RIO_IS_WIN
};

}

#endif // RIO_GPU_RENDER_BUFFER_H
