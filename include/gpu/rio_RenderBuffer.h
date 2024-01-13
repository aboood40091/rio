#ifndef RIO_GPU_RENDER_BUFFER_H
#define RIO_GPU_RENDER_BUFFER_H

#include <gfx/rio_Color.h>
#include <gfx/rio_Graphics.h>
#include <gpu/rio_Texture.h>

namespace rio {

class RenderTargetColor;
class RenderTargetDepth;

class RenderBuffer
{
public:
    RenderBuffer();
    RenderBuffer(u32 w, u32 h);
    RenderBuffer(const BaseVec2i& size);
#if RIO_IS_WIN
    ~RenderBuffer();
#endif // RIO_IS_WIN

private:
    RenderBuffer(const RenderBuffer&);
    RenderBuffer& operator=(const RenderBuffer&);

public:
    RenderTargetColor* getRenderTargetColor(u32 index = 0) const
    {
        RIO_ASSERT(index < Graphics::RENDER_TARGET_MAX_NUM);
        return mpColorTarget[index];
    }

    RenderTargetDepth* getRenderTargetDepth() const
    {
        return mpDepthTarget;
    }

    void setRenderTargetColor(RenderTargetColor* target, u32 index = 0)
    {
        RIO_ASSERT(index < Graphics::RENDER_TARGET_MAX_NUM);
        mpColorTarget[index] = target;
    }

    void setRenderTargetDepth(RenderTargetDepth* target)
    {
        mpDepthTarget = target;
    }

    void setRenderTargetColorNullAll()
    {
        for (u32 i = 0; i < Graphics::RENDER_TARGET_MAX_NUM; i++)
            setRenderTargetColorNull(i);
    }

    void setRenderTargetColorNull(u32 index = 0) { setRenderTargetColor(nullptr, index); }
    void setRenderTargetDepthNull() { setRenderTargetDepth(nullptr); }

    const BaseVec2i& getSize() const
    {
        return mSize;
    }

    void setSize(u32 w, u32 h)
    {
        mSize.x = w;
        mSize.y = h;
        resetScissor();
    }

    void setSize(const BaseVec2i& size)
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

    void setScissor(const BaseVec2i& pos, const BaseVec2i& size)
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

    enum ClearFlag
    {
        CLEAR_FLAG_COLOR    = 1 << 0,
        CLEAR_FLAG_DEPTH    = 1 << 1,
        CLEAR_FLAG_STENCIL  = 1 << 2,

        CLEAR_FLAG_COLOR_DEPTH          = CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH                     ,
        CLEAR_FLAG_COLOR_DEPTH_STENCIL  = CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH | CLEAR_FLAG_STENCIL,
        CLEAR_FLAG_DEPTH_STENCIL        =                    CLEAR_FLAG_DEPTH | CLEAR_FLAG_STENCIL
    };

    void clear(u32 color_target_index, u32 clear_flag, const Color4f& color = Color4f::cBlack, f32 depth = 1.0f, u8 stencil = 0);
    void clear(u32 clear_flag, const Color4f& color = Color4f::cBlack, f32 depth = 1.0f, u8 stencil = 0) { clear(0, clear_flag, color, depth, stencil); }

    bool read(
        u32 color_target_index, void* pixels
#if RIO_IS_WIN
        , u32 width
        , u32 height
        , const NativeTextureFormat& native_format
#endif // RIO_IS_WIN
    );

private:
#if RIO_IS_WIN
    void bindFBO_() const;
#endif // RIO_IS_WIN
    void bindRenderTargetColor_() const;
    void bindRenderTargetDepth_() const;

private:
    BaseVec2i           mSize;
    BaseVec2i           mScissorPos;
    BaseVec2i           mScissorSize;
    RenderTargetColor*  mpColorTarget[Graphics::RENDER_TARGET_MAX_NUM];
    RenderTargetDepth*  mpDepthTarget;
#if RIO_IS_WIN
    u32                 mHandle;
    mutable u32         mDrawBuffers[Graphics::RENDER_TARGET_MAX_NUM];

    void createHandle_();
#endif // RIO_IS_WIN
};

}

#endif // RIO_GPU_RENDER_BUFFER_H
