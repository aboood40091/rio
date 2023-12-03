#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <audio/rio_AudioMgr.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_RenderState.h>

#include <coreinit/foreground.h>
#include <coreinit/memdefaultheap.h>
#include <coreinit/memfrmheap.h>
#include <gx2/clear.h>
#include <gx2/display.h>
#include <gx2/event.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/state.h>
#include <gx2/swap.h>
#include <proc_ui/procui.h>

#include <whb/crash.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>

extern "C" void OSBlockThreadsOnExit(void);
extern "C" void GX2ConvertDepthBufferToTextureSurface(const GX2DepthBuffer* src_buffer, GX2Surface* dst_surface, u32 dst_mip, u32 dst_slice);

namespace rio {

bool Window::foregroundAcquire_()
{
#ifdef RIO_DEBUG
    if (mNativeWindow.mIsRunning)
    {
        WHBInitCrashHandler();
        WHBLogCafeInit();
        WHBLogUdpInit();
    }
#endif // RIO_DEBUG

    // Get the MEM1 heap and Foreground Bucket heap handles
    mNativeWindow.mHeapHandle_MEM1 = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM1);
    mNativeWindow.mHeapHandle_Fg = MEMGetBaseHeapHandle(MEM_BASE_HEAP_FG);

    if (!(mNativeWindow.mHeapHandle_MEM1 && mNativeWindow.mHeapHandle_Fg))
        return false;

    // Allocate TV scan buffer
    {
        GX2TVRenderMode tv_render_mode;

        // Get current TV scan mode
        GX2TVScanMode tv_scan_mode = GX2GetSystemTVScanMode();

        // Determine TV framebuffer dimensions (scan buffer, specifically)
        if (tv_scan_mode != GX2_TV_SCAN_MODE_576I && tv_scan_mode != GX2_TV_SCAN_MODE_480I
            && mWidth >= 1920 && mHeight >= 1080)
        {
            mWidth = 1920;
            mHeight = 1080;
            tv_render_mode = GX2_TV_RENDER_MODE_WIDE_1080P;
        }
        else if (mWidth >= 1280 && mHeight >= 720)
        {
            mWidth = 1280;
            mHeight = 720;
            tv_render_mode = GX2_TV_RENDER_MODE_WIDE_720P;
        }
        else if (mWidth >= 850 && mHeight >= 480)
        {
            mWidth = 854;
            mHeight = 480;
            tv_render_mode = GX2_TV_RENDER_MODE_WIDE_480P;
        }
        else // if (mWidth >= 640 && mHeight >= 480)
        {
            mWidth = 640;
            mHeight = 480;
            tv_render_mode = GX2_TV_RENDER_MODE_STANDARD_480P;
        }

        // Calculate TV scan buffer byte size
        u32 tv_scan_buffer_size, unk;
        GX2CalcTVSize(
            tv_render_mode,                       // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE,            // Two buffers for double-buffering
            &tv_scan_buffer_size,                 // Output byte size
            &unk                                  // Unknown; seems like we have no use for it
        );

        // Allocate TV scan buffer
        mNativeWindow.mpTvScanBuffer = MEMAllocFromFrmHeapEx(
            mNativeWindow.mHeapHandle_Fg,
            tv_scan_buffer_size,
            GX2_SCAN_BUFFER_ALIGNMENT // Required alignment
        );

        if (!mNativeWindow.mpTvScanBuffer)
            return false;

        // Flush allocated buffer from CPU cache
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.mpTvScanBuffer, tv_scan_buffer_size);

        // Set the current TV scan buffer
        GX2SetTVBuffer(
            mNativeWindow.mpTvScanBuffer,           // Scan Buffer
            tv_scan_buffer_size,                    // Scan Buffer Size
            tv_render_mode,                         // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,   // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE               // Enable double-buffering
        );

        // Set the current TV scan buffer dimensions
        GX2SetTVScale(mWidth, mHeight);
    }

    // Allocate DRC (Gamepad) scan buffer
    {
        u32 drc_width = 854;
        u32 drc_height = 480;

        // Calculate DRC scan buffer byte size
        u32 drc_scan_buffer_size, unk;
        GX2CalcDRCSize(
            GX2_DRC_RENDER_MODE_SINGLE,           // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE,            // Two buffers for double-buffering
            &drc_scan_buffer_size,                // Output byte size
            &unk                                  // Unknown; seems like we have no use for it
        );

        // Allocate DRC scan buffer
        mNativeWindow.mpDrcScanBuffer = MEMAllocFromFrmHeapEx(
            mNativeWindow.mHeapHandle_Fg,
            drc_scan_buffer_size,
            GX2_SCAN_BUFFER_ALIGNMENT // Required alignment
        );

        if (!mNativeWindow.mpDrcScanBuffer)
            return false;

        // Flush allocated buffer from CPU cache
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.mpDrcScanBuffer, drc_scan_buffer_size);

        // Set the current DRC scan buffer
        GX2SetDRCBuffer(
            mNativeWindow.mpDrcScanBuffer,          // Scan Buffer
            drc_scan_buffer_size,                   // Scan Buffer Size
            GX2_DRC_RENDER_MODE_SINGLE,             // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,   // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE               // Enable double-buffering
        );

        // Set the current DRC scan buffer dimensions
        GX2SetDRCScale(drc_width, drc_height);
    }

    // Initialize color buffer
    mNativeWindow.mColorBuffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    mNativeWindow.mColorBuffer.surface.width = mWidth;
    mNativeWindow.mColorBuffer.surface.height = mHeight;
    mNativeWindow.mColorBuffer.surface.depth = 1;
    mNativeWindow.mColorBuffer.surface.mipLevels = 1;
    mNativeWindow.mColorBuffer.surface.format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
    mNativeWindow.mColorBuffer.surface.aa = GX2_AA_MODE1X;
    mNativeWindow.mColorBuffer.surface.use = GX2_SURFACE_USE_TEXTURE_COLOR_BUFFER_TV;
    mNativeWindow.mColorBuffer.surface.mipmaps = nullptr;
    mNativeWindow.mColorBuffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    mNativeWindow.mColorBuffer.surface.swizzle = 0;
    mNativeWindow.mColorBuffer.viewMip = 0;
    mNativeWindow.mColorBuffer.viewFirstSlice = 0;
    mNativeWindow.mColorBuffer.viewNumSlices = 1;
    GX2CalcSurfaceSizeAndAlignment(&mNativeWindow.mColorBuffer.surface);
    GX2InitColorBufferRegs(&mNativeWindow.mColorBuffer);

    // Allocate color buffer data
    mNativeWindow.mpColorBufferImageData = MEMAllocFromFrmHeapEx(
        mNativeWindow.mHeapHandle_MEM1,
        mNativeWindow.mColorBuffer.surface.imageSize, // Data byte size
        mNativeWindow.mColorBuffer.surface.alignment  // Required alignment
    );

    if (!mNativeWindow.mpColorBufferImageData)
        return false;

    mNativeWindow.mColorBuffer.surface.image = mNativeWindow.mpColorBufferImageData;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE, mNativeWindow.mpColorBufferImageData, mNativeWindow.mColorBuffer.surface.imageSize);

    // Copy color buffer to a texture
    mNativeWindow.mColorBufferTexture.surface = mNativeWindow.mColorBuffer.surface;
    mNativeWindow.mColorBufferTexture.surface.use = GX2_SURFACE_USE_TEXTURE;
    mNativeWindow.mColorBufferTexture.viewFirstMip = 0;
    mNativeWindow.mColorBufferTexture.viewNumMips = 1;
    mNativeWindow.mColorBufferTexture.viewFirstSlice = 0;
    mNativeWindow.mColorBufferTexture.viewNumSlices = 1;
    mNativeWindow.mColorBufferTexture.compMap = 0x00010203;
    GX2InitTextureRegs(&mNativeWindow.mColorBufferTexture);

    // Initialize depth buffer
    mNativeWindow.mDepthBuffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    mNativeWindow.mDepthBuffer.surface.width = mWidth;
    mNativeWindow.mDepthBuffer.surface.height = mHeight;
    mNativeWindow.mDepthBuffer.surface.depth = 1;
    mNativeWindow.mDepthBuffer.surface.mipLevels = 1;
    mNativeWindow.mDepthBuffer.surface.format = GX2_SURFACE_FORMAT_FLOAT_D24_S8;
    mNativeWindow.mDepthBuffer.surface.aa = GX2_AA_MODE1X;
    mNativeWindow.mDepthBuffer.surface.use = GX2_SURFACE_USE_DEPTH_BUFFER;
    mNativeWindow.mDepthBuffer.surface.mipmaps = nullptr;
    mNativeWindow.mDepthBuffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    mNativeWindow.mDepthBuffer.surface.swizzle = 0;
    mNativeWindow.mDepthBuffer.viewMip = 0;
    mNativeWindow.mDepthBuffer.viewFirstSlice = 0;
    mNativeWindow.mDepthBuffer.viewNumSlices = 1;
    mNativeWindow.mDepthBuffer.hiZPtr = nullptr;
    mNativeWindow.mDepthBuffer.hiZSize = 0;
    mNativeWindow.mDepthBuffer.depthClear = 1.0f;
    mNativeWindow.mDepthBuffer.stencilClear = 0;
    GX2CalcSurfaceSizeAndAlignment(&mNativeWindow.mDepthBuffer.surface);
    GX2InitDepthBufferRegs(&mNativeWindow.mDepthBuffer);

    // Allocate depth buffer data
    mNativeWindow.mpDepthBufferImageData = MEMAllocFromFrmHeapEx(
        mNativeWindow.mHeapHandle_MEM1,
        mNativeWindow.mDepthBuffer.surface.imageSize, // Data byte size
        mNativeWindow.mDepthBuffer.surface.alignment  // Required alignment
    );

    if (!mNativeWindow.mpDepthBufferImageData)
        return false;

    mNativeWindow.mDepthBuffer.surface.image = mNativeWindow.mpDepthBufferImageData;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.mpDepthBufferImageData, mNativeWindow.mDepthBuffer.surface.imageSize);

    // Initialize depth buffer texture
    mNativeWindow.mDepthBufferTexture.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    mNativeWindow.mDepthBufferTexture.surface.width = mWidth;
    mNativeWindow.mDepthBufferTexture.surface.height = mHeight;
    mNativeWindow.mDepthBufferTexture.surface.depth = 1;
    mNativeWindow.mDepthBufferTexture.surface.mipLevels = 1;
    mNativeWindow.mDepthBufferTexture.surface.format = GX2_SURFACE_FORMAT_FLOAT_R32;
    mNativeWindow.mDepthBufferTexture.surface.aa = GX2_AA_MODE1X;
    mNativeWindow.mDepthBufferTexture.surface.use = GX2_SURFACE_USE_TEXTURE;
    mNativeWindow.mDepthBufferTexture.surface.mipmaps = nullptr;
    mNativeWindow.mDepthBufferTexture.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    mNativeWindow.mDepthBufferTexture.surface.swizzle = 0;
    mNativeWindow.mDepthBufferTexture.viewFirstMip = 0;
    mNativeWindow.mDepthBufferTexture.viewNumMips = 1;
    mNativeWindow.mDepthBufferTexture.viewFirstSlice = 0;
    mNativeWindow.mDepthBufferTexture.viewNumSlices = 1;
    mNativeWindow.mDepthBufferTexture.compMap = 0x00040405;
    GX2CalcSurfaceSizeAndAlignment(&mNativeWindow.mDepthBufferTexture.surface);
    GX2InitTextureRegs(&mNativeWindow.mDepthBufferTexture);

    // Allocate depth buffer texture data
    mNativeWindow.mpDepthBufferTextureImageData = MEMAllocFromFrmHeapEx(
        mNativeWindow.mHeapHandle_MEM1,
        mNativeWindow.mDepthBufferTexture.surface.imageSize, // Data byte size
        mNativeWindow.mDepthBufferTexture.surface.alignment  // Required alignment
    );

    if (!mNativeWindow.mpDepthBufferTextureImageData)
        return false;

    mNativeWindow.mDepthBufferTexture.surface.image = mNativeWindow.mpDepthBufferTextureImageData;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE, mNativeWindow.mpDepthBufferTextureImageData, mNativeWindow.mDepthBufferTexture.surface.imageSize);

    // Enable TV and DRC
    GX2SetTVEnable(true);
    GX2SetDRCEnable(true);

    // If not first time in foreground, restore the GX2 context state
    if (mNativeWindow.mIsRunning)
    {
        GX2SetContextState(mNativeWindow.mpContextState);
        GX2SetColorBuffer(&mNativeWindow.mColorBuffer, GX2_RENDER_TARGET_0);
        GX2SetDepthBuffer(&mNativeWindow.mDepthBuffer);
    }

    // Initialize GQR2 to GQR5
    asm volatile ("mtspr %0, %1" : : "i" (898), "r" (0x00040004));
    asm volatile ("mtspr %0, %1" : : "i" (899), "r" (0x00050005));
    asm volatile ("mtspr %0, %1" : : "i" (900), "r" (0x00060006));
    asm volatile ("mtspr %0, %1" : : "i" (901), "r" (0x00070007));

    return true;
}

void Window::foregroundRelease_()
{
    if (mNativeWindow.mIsRunning)
        ProcUIDrawDoneRelease();

    if (mNativeWindow.mHeapHandle_Fg)
    {
        MEMFreeToFrmHeap(mNativeWindow.mHeapHandle_Fg, MEM_FRM_HEAP_FREE_ALL);
        mNativeWindow.mHeapHandle_Fg = nullptr;
    }
    mNativeWindow.mpTvScanBuffer = nullptr;
    mNativeWindow.mpDrcScanBuffer = nullptr;

    if (mNativeWindow.mHeapHandle_MEM1)
    {
        MEMFreeToFrmHeap(mNativeWindow.mHeapHandle_MEM1, MEM_FRM_HEAP_FREE_ALL);
        mNativeWindow.mHeapHandle_MEM1 = nullptr;
    }
    mNativeWindow.mpColorBufferImageData = nullptr;
    mNativeWindow.mpDepthBufferImageData = nullptr;
    mNativeWindow.mpDepthBufferTextureImageData = nullptr;

#ifdef RIO_DEBUG
    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
#endif // RIO_DEBUG
}

bool Window::initialize_()
{
    // Allocate GX2 command buffer
    mNativeWindow.mpCmdList = MEMAllocFromDefaultHeapEx(
        0x400000,                    // A very commonly used size in Nintendo games
        GX2_COMMAND_BUFFER_ALIGNMENT // Required alignment
    );

    if (!mNativeWindow.mpCmdList)
    {
        terminate_();
        return false;
    }

    // Several parameters to initialize GX2 with
    u32 initAttribs[] = {
        GX2_INIT_CMD_BUF_BASE, (uintptr_t)mNativeWindow.mpCmdList,  // Command Buffer Base Address
        GX2_INIT_CMD_BUF_POOL_SIZE, 0x400000,                       // Command Buffer Size
        GX2_INIT_ARGC, 0,                                           // main() arguments count
        GX2_INIT_ARGV, (uintptr_t)nullptr,                          // main() arguments vector
        GX2_INIT_END                                                // Ending delimiter
    };

    // Initialize GX2
    GX2Init(initAttribs);

    // Create TV and DRC scan, color and depth buffers
    if (!foregroundAcquire_())
    {
        terminate_();
        return false;
    }

    // Allocate context state instance
    mNativeWindow.mpContextState = (GX2ContextState*)MEMAllocFromDefaultHeapEx(
        sizeof(GX2ContextState),    // Size of context
        GX2_CONTEXT_STATE_ALIGNMENT // Required alignment
    );

    if (!mNativeWindow.mpContextState)
    {
        terminate_();
        return false;
    }

    // Initialize it to default state
    GX2SetupContextStateEx(mNativeWindow.mpContextState, false);

    // Make context of window current
    makeContextCurrent();

    // Set swap interval to 1 by default
    setSwapInterval(1);

    // Scissor test is always enabled in GX2

    // Initialize ProcUI
    ProcUIInit(&OSSavesDone_ReadyToRelease);

    mNativeWindow.mIsRunning = true;
    return true;
}

bool Window::isRunning() const
{
    return mNativeWindow.mIsRunning;
}

void Window::terminate_()
{
    mNativeWindow.mIsRunning = false;

    if (mNativeWindow.mpCmdList)
    {
        MEMFreeToDefaultHeap(mNativeWindow.mpCmdList);
        mNativeWindow.mpCmdList = nullptr;
    }

    if (mNativeWindow.mpContextState)
    {
        MEMFreeToDefaultHeap(mNativeWindow.mpContextState);
        mNativeWindow.mpContextState = nullptr;
    }

    AudioMgr::destroySingleton();

    foregroundRelease_();

    OSBlockThreadsOnExit();
    _Exit(-1);
}

void Window::makeContextCurrent() const
{
    GX2SetContextState(mNativeWindow.mpContextState);
    GX2SetColorBuffer(&mNativeWindow.mColorBuffer, GX2_RENDER_TARGET_0);
    GX2SetDepthBuffer(&mNativeWindow.mDepthBuffer);
}

void Window::setSwapInterval(u32 swap_interval)
{
    GX2SetSwapInterval(swap_interval);
}

void Window::swapBuffers() const
{
    // Disable Depth test to achieve same behavior as the MS Windows version
    RenderState render_state;
    render_state.setDepthTestEnable(false);
    render_state.apply();

    // Make sure to flush all commands to GPU before copying the color buffer to the scan buffers
    // (Calling GX2DrawDone instead here causes slow downs)
    GX2Flush();

    // Copy the color buffer to the TV and DRC scan buffers
    GX2CopyColorBufferToScanBuffer(&mNativeWindow.mColorBuffer, GX2_SCAN_TARGET_TV);
    GX2CopyColorBufferToScanBuffer(&mNativeWindow.mColorBuffer, GX2_SCAN_TARGET_DRC);
    // Flip
    GX2SwapScanBuffers();

    // Reset context state for next frame
    GX2SetContextState(mNativeWindow.mpContextState);

    // Flush all commands to GPU before GX2WaitForFlip since it will block the CPU
    GX2Flush();

    // Make sure TV and DRC are enabled
    GX2SetTVEnable(true);
    GX2SetDRCEnable(true);

    // Wait until swapping is done
    GX2WaitForFlip();

    // ProcUI
    ProcUIStatus status = ProcUIProcessMessages(true);
    ProcUIStatus previous_status = status;

    if (status == PROCUI_STATUS_RELEASE_FOREGROUND)
    {
        const_cast<Window*>(this)->foregroundRelease_();
        status = ProcUIProcessMessages(true);
    }

    if (status == PROCUI_STATUS_EXITING ||
        (previous_status == PROCUI_STATUS_RELEASE_FOREGROUND &&
         status == PROCUI_STATUS_IN_FOREGROUND &&
         !const_cast<Window*>(this)->foregroundAcquire_()))
    {
        ProcUIShutdown();
        const_cast<Window*>(this)->terminate_();
    }
}

void Window::clearColor(f32 r, f32 g, f32 b, f32 a)
{
    // Clear using the given color
    // Does not need a current context to be set
    GX2ClearColor(&mNativeWindow.mColorBuffer, r, g, b, a);

    // GX2ClearColor invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearDepth()
{
    // Clear
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.mDepthBuffer,
                           mNativeWindow.mDepthBuffer.depthClear,
                           0,
                           GX2_CLEAR_FLAGS_DEPTH);

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearDepth(f32 depth)
{
    // Clear using the given depth
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.mDepthBuffer,
                           depth,
                           0,
                           GX2_CLEAR_FLAGS_DEPTH);

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearStencil()
{
    // Clear
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.mDepthBuffer,
                           0,
                           mNativeWindow.mDepthBuffer.stencilClear,
                           GX2_CLEAR_FLAGS_STENCIL);

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearStencil(u8 stencil)
{
    // Clear using the given stencil
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.mDepthBuffer,
                           0,
                           stencil,
                           GX2_CLEAR_FLAGS_STENCIL);

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearDepthStencil()
{
    // Clear
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.mDepthBuffer,
                           mNativeWindow.mDepthBuffer.depthClear,
                           mNativeWindow.mDepthBuffer.stencilClear,
                           (GX2ClearFlags)(GX2_CLEAR_FLAGS_DEPTH |
                                           GX2_CLEAR_FLAGS_STENCIL));

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearDepthStencil(f32 depth, u8 stencil)
{
    // Clear using the given depth-stencil
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.mDepthBuffer,
                           depth,
                           stencil,
                           (GX2ClearFlags)(GX2_CLEAR_FLAGS_DEPTH |
                                           GX2_CLEAR_FLAGS_STENCIL));

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::updateDepthBufferTexture_()
{
    GX2Invalidate(GX2_INVALIDATE_MODE_DEPTH_BUFFER, mNativeWindow.mDepthBuffer.surface.image, mNativeWindow.mDepthBuffer.surface.imageSize);

    GX2ConvertDepthBufferToTextureSurface(
        &mNativeWindow.mDepthBuffer,
        &mNativeWindow.mDepthBufferTexture.surface,
        0,
        0
    );

    GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, mNativeWindow.mDepthBufferTexture.surface.image, mNativeWindow.mDepthBufferTexture.surface.imageSize);
}

}

#endif // RIO_IS_CAFE
