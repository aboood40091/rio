#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <audio/rio_AudioMgr.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_RenderState.h>

#include <coreinit/foreground.h>
#include <coreinit/memdefaultheap.h>
#include <coreinit/memfrmheap.h>
#include <coreinit/memheap.h>
#include <gx2/clear.h>
#include <gx2/context.h>
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

namespace {

static void* gCmdlist = nullptr;
static GX2ContextState* gContext = nullptr;
static void* gTvScanBuffer = nullptr;
static void* gDrcScanBuffer = nullptr;
static GX2ColorBuffer gColorBuffer;
static GX2Texture gColorBufferTexture;
static void* gColorBufferImageData = nullptr;
static GX2DepthBuffer gDepthBuffer;
static void* gDepthBufferImageData = nullptr;
static MEMHeapHandle gMEM1HeapHandle;
static MEMHeapHandle gFgHeapHandle;

static bool gIsRunning = false;

}

namespace rio {

bool Window::foregroundAcquire_()
{
    if (gIsRunning)
    {
        WHBInitCrashHandler();
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    // Get the MEM1 heap and Foreground Bucket heap handles
    gMEM1HeapHandle = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM1);
    gFgHeapHandle = MEMGetBaseHeapHandle(MEM_BASE_HEAP_FG);

    if (!(gMEM1HeapHandle && gFgHeapHandle))
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
        gTvScanBuffer = MEMAllocFromFrmHeapEx(
            gFgHeapHandle,
            tv_scan_buffer_size,
            GX2_SCAN_BUFFER_ALIGNMENT // Required alignment
        );

        if (!gTvScanBuffer)
            return false;

        // Flush allocated buffer from CPU cache
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, gTvScanBuffer, tv_scan_buffer_size);

        // Set the current TV scan buffer
        GX2SetTVBuffer(
            gTvScanBuffer,                        // Scan Buffer
            tv_scan_buffer_size,                  // Scan Buffer Size
            tv_render_mode,                       // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE             // Enable double-buffering
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
        gDrcScanBuffer = MEMAllocFromFrmHeapEx(
            gFgHeapHandle,
            drc_scan_buffer_size,
            GX2_SCAN_BUFFER_ALIGNMENT // Required alignment
        );

        if (!gDrcScanBuffer)
            return false;

        // Flush allocated buffer from CPU cache
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, gDrcScanBuffer, drc_scan_buffer_size);

        // Set the current DRC scan buffer
        GX2SetDRCBuffer(
            gDrcScanBuffer,                       // Scan Buffer
            drc_scan_buffer_size,                 // Scan Buffer Size
            GX2_DRC_RENDER_MODE_SINGLE,           // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE             // Enable double-buffering
        );

        // Set the current DRC scan buffer dimensions
        GX2SetDRCScale(drc_width, drc_height);
    }

    // Initialize color buffer
    gColorBuffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    gColorBuffer.surface.width = mWidth;
    gColorBuffer.surface.height = mHeight;
    gColorBuffer.surface.depth = 1;
    gColorBuffer.surface.mipLevels = 1;
    gColorBuffer.surface.format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
    gColorBuffer.surface.aa = GX2_AA_MODE1X;
    gColorBuffer.surface.use = GX2_SURFACE_USE_TEXTURE_COLOR_BUFFER_TV;
    gColorBuffer.surface.mipmaps = nullptr;
    gColorBuffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    gColorBuffer.surface.swizzle  = 0;
    gColorBuffer.viewMip = 0;
    gColorBuffer.viewFirstSlice = 0;
    gColorBuffer.viewNumSlices = 1;
    GX2CalcSurfaceSizeAndAlignment(&gColorBuffer.surface);
    GX2InitColorBufferRegs(&gColorBuffer);

    // Allocate color buffer data
    gColorBufferImageData = MEMAllocFromFrmHeapEx(
        gMEM1HeapHandle,
        gColorBuffer.surface.imageSize, // Data byte size
        gColorBuffer.surface.alignment  // Required alignment
    );

    if (!gColorBufferImageData)
        return false;

    gColorBuffer.surface.image = gColorBufferImageData;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, gColorBufferImageData, gColorBuffer.surface.imageSize);

    // Copy color buffer to a texture
    gColorBufferTexture.surface = gColorBuffer.surface;
    gColorBufferTexture.surface.use = GX2_SURFACE_USE_TEXTURE;
    gColorBufferTexture.viewFirstMip = 0;
    gColorBufferTexture.viewNumMips = 1;
    gColorBufferTexture.viewFirstSlice = 0;
    gColorBufferTexture.viewNumSlices = 1;
    gColorBufferTexture.compMap = 0x00010203;
    GX2InitTextureRegs(&gColorBufferTexture);

    // Initialize depth buffer
    gDepthBuffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    gDepthBuffer.surface.width = mWidth;
    gDepthBuffer.surface.height = mHeight;
    gDepthBuffer.surface.depth = 1;
    gDepthBuffer.surface.mipLevels = 1;
    gDepthBuffer.surface.format = GX2_SURFACE_FORMAT_FLOAT_D24_S8;
    gDepthBuffer.surface.aa = GX2_AA_MODE1X;
    gDepthBuffer.surface.use = GX2_SURFACE_USE_TEXTURE | GX2_SURFACE_USE_DEPTH_BUFFER;
    gDepthBuffer.surface.mipmaps = nullptr;
    gDepthBuffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    gDepthBuffer.surface.swizzle  = 0;
    gDepthBuffer.viewMip = 0;
    gDepthBuffer.viewFirstSlice = 0;
    gDepthBuffer.viewNumSlices = 1;
    gDepthBuffer.hiZPtr = nullptr;
    gDepthBuffer.hiZSize = 0;
    gDepthBuffer.depthClear = 1.0f;
    gDepthBuffer.stencilClear = 0;
    GX2CalcSurfaceSizeAndAlignment(&gDepthBuffer.surface);
    GX2InitDepthBufferRegs(&gDepthBuffer);

    // Allocate depth buffer data
    gDepthBufferImageData = MEMAllocFromFrmHeapEx(
        gMEM1HeapHandle,
        gDepthBuffer.surface.imageSize, // Data byte size
        gDepthBuffer.surface.alignment  // Required alignment
    );

    if (!gDepthBufferImageData)
        return false;

    gDepthBuffer.surface.image = gDepthBufferImageData;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, gDepthBufferImageData, gDepthBuffer.surface.imageSize);

    // Enable TV and DRC
    GX2SetTVEnable(true);
    GX2SetDRCEnable(true);

    // If not first time in foreground, restore the GX2 context state
    if (gIsRunning)
    {
        GX2SetContextState(gContext);
        GX2SetColorBuffer(&gColorBuffer, GX2_RENDER_TARGET_0);
        GX2SetDepthBuffer(&gDepthBuffer);
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
    if (gIsRunning)
        ProcUIDrawDoneRelease();

    if (gFgHeapHandle)
    {
        MEMFreeToFrmHeap(gFgHeapHandle, MEM_FRM_HEAP_FREE_ALL);
        gFgHeapHandle = nullptr;
    }
    gTvScanBuffer = nullptr;
    gDrcScanBuffer = nullptr;

    if (gMEM1HeapHandle)
    {
        MEMFreeToFrmHeap(gMEM1HeapHandle, MEM_FRM_HEAP_FREE_ALL);
        gMEM1HeapHandle = nullptr;
    }
    gColorBufferImageData = nullptr;
    gDepthBufferImageData = nullptr;

    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
}

bool Window::initialize_()
{
    // Allocate GX2 command buffer
    gCmdlist = MEMAllocFromDefaultHeapEx(
        0x400000,                    // A very commonly used size in Nintendo games
        GX2_COMMAND_BUFFER_ALIGNMENT // Required alignment
    );

    if (!gCmdlist)
    {
        terminate_();
        return false;
    }

    // Several parameters to initialize GX2 with
    u32 initAttribs[] = {
        GX2_INIT_CMD_BUF_BASE, (uintptr_t)gCmdlist, // Command Buffer Base Address
        GX2_INIT_CMD_BUF_POOL_SIZE, 0x400000,       // Command Buffer Size
        GX2_INIT_ARGC, 0,                           // main() arguments count
        GX2_INIT_ARGV, (uintptr_t)nullptr,          // main() arguments vector
        GX2_INIT_END                                // Ending delimiter
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
    gContext = (GX2ContextState*)MEMAllocFromDefaultHeapEx(
        sizeof(GX2ContextState),    // Size of context
        GX2_CONTEXT_STATE_ALIGNMENT // Required alignment
    );

    if (!gContext)
    {
        terminate_();
        return false;
    }

    // Initialize it to default state
    GX2SetupContextStateEx(gContext, false);

    // Make context of window current
    makeContextCurrent();

    // Set swap interval to 1 by default
    setSwapInterval(1);

    // Scissor test is always enabled in GX2

    // Initialize ProcUI
    ProcUIInit(&OSSavesDone_ReadyToRelease);

    gIsRunning = true;
    return true;
}

bool Window::isRunning() const
{
    return gIsRunning;
}

void Window::terminate_()
{
    gIsRunning = false;

    if (gCmdlist)
    {
        MEMFreeToDefaultHeap(gCmdlist);
        gCmdlist = nullptr;
    }

    if (gContext)
    {
        MEMFreeToDefaultHeap(gContext);
        gContext = nullptr;
    }

    AudioMgr::destroySingleton();

    foregroundRelease_();

    OSBlockThreadsOnExit();
    _Exit(-1);
}

void Window::makeContextCurrent() const
{
    GX2SetContextState(gContext);
    GX2SetColorBuffer(&gColorBuffer, GX2_RENDER_TARGET_0);
    GX2SetDepthBuffer(&gDepthBuffer);
}

void Window::setSwapInterval(u32 swap_interval)
{
    GX2SetSwapInterval(swap_interval);
}

void Window::swapBuffers() const
{
    // Disable Depth test to achieve same behavior as the MS Windows version
    rio::RenderState render_state;
    render_state.setDepthTestEnable(false);
    render_state.apply();

    // Make sure to flush all commands to GPU before copying the color buffer to the scan buffers
    // (Calling GX2DrawDone instead here causes slow downs)
    GX2Flush();

    // Copy the color buffer to the TV and DRC scan buffers
    GX2CopyColorBufferToScanBuffer(&gColorBuffer, GX2_SCAN_TARGET_TV);
    GX2CopyColorBufferToScanBuffer(&gColorBuffer, GX2_SCAN_TARGET_DRC);
    // Flip
    GX2SwapScanBuffers();

    // Reset context state for next frame
    GX2SetContextState(gContext);

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
    GX2ClearColor(&gColorBuffer, r, g, b, a);

    // GX2ClearColor invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearDepth()
{
    // Clear
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&gDepthBuffer,
                           gDepthBuffer.depthClear,
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
    GX2ClearDepthStencilEx(&gDepthBuffer,
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
    GX2ClearDepthStencilEx(&gDepthBuffer,
                           0,
                           gDepthBuffer.stencilClear,
                           GX2_CLEAR_FLAGS_STENCIL);

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearStencil(u8 stencil)
{
    // Clear using the given stencil
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&gDepthBuffer,
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
    GX2ClearDepthStencilEx(&gDepthBuffer,
                           gDepthBuffer.depthClear,
                           gDepthBuffer.stencilClear,
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
    GX2ClearDepthStencilEx(&gDepthBuffer,
                           depth,
                           stencil,
                           (GX2ClearFlags)(GX2_CLEAR_FLAGS_DEPTH |
                                           GX2_CLEAR_FLAGS_STENCIL));

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void* Window::getWindowInner()
{
    return nullptr;
}

NativeTexture2DHandle Window::getWindowColorBuffer()
{
    return &gColorBufferTexture;
}

}

#endif // RIO_IS_CAFE
