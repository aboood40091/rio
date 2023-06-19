#ifndef RIO_GFX_NATIVE_WINDOW_CAFE_H
#define RIO_GFX_NATIVE_WINDOW_CAFE_H

#include <misc/rio_Types.h>

#include <coreinit/memheap.h>
#include <gx2/context.h>
#include <gx2/texture.h>

namespace rio {

class Window;

class NativeWindow
{
public:
    NativeWindow()
        : mpCmdList(nullptr)
        , mpContextState(nullptr)
        , mpTvScanBuffer(nullptr)
        , mpDrcScanBuffer(nullptr)
        , mpColorBufferImageData(nullptr)
        , mpDepthBufferImageData(nullptr)
        , mIsRunning(false)
    {
    }

    GX2ContextState* getContextState() const { return mpContextState; }

    const GX2ColorBuffer& getColorBuffer() const { return mColorBuffer; }
    const GX2Texture& getColorBufferTexture() const { return mColorBufferTexture; }

    const GX2DepthBuffer& getDepthBuffer() const { return mDepthBuffer; }
    const GX2Texture& getDepthBufferTexture() const { return mDepthBufferTexture; }

private:
    void* mpCmdList;

    GX2ContextState* mpContextState;

    void* mpTvScanBuffer;
    void* mpDrcScanBuffer;

    GX2ColorBuffer mColorBuffer;
    void* mpColorBufferImageData;

    GX2Texture mColorBufferTexture;

    GX2DepthBuffer mDepthBuffer;
    void* mpDepthBufferImageData;

    GX2Texture mDepthBufferTexture;
    void* mpDepthBufferTextureImageData;

    MEMHeapHandle mHeapHandle_MEM1;
    MEMHeapHandle mHeapHandle_Fg;

    bool mIsRunning;

    friend class Window;
};

}

#endif // RIO_GFX_NATIVE_WINDOW_CAFE_H
