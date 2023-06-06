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

namespace rio {

bool Window::foregroundAcquire_()
{
#ifdef RIO_DEBUG
    if (mNativeWindow.is_running)
    {
        WHBInitCrashHandler();
        WHBLogCafeInit();
        WHBLogUdpInit();
    }
#endif // RIO_DEBUG

    // Get the MEM1 heap and Foreground Bucket heap handles
    mNativeWindow.heap_handle_MEM1 = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM1);
    mNativeWindow.heap_handle_Fg = MEMGetBaseHeapHandle(MEM_BASE_HEAP_FG);

    if (!(mNativeWindow.heap_handle_MEM1 && mNativeWindow.heap_handle_Fg))
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
        mNativeWindow.p_tv_scan_buffer = MEMAllocFromFrmHeapEx(
            mNativeWindow.heap_handle_Fg,
            tv_scan_buffer_size,
            GX2_SCAN_BUFFER_ALIGNMENT // Required alignment
        );

        if (!mNativeWindow.p_tv_scan_buffer)
            return false;

        // Flush allocated buffer from CPU cache
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.p_tv_scan_buffer, tv_scan_buffer_size);

        // Set the current TV scan buffer
        GX2SetTVBuffer(
            mNativeWindow.p_tv_scan_buffer,         // Scan Buffer
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
        mNativeWindow.p_drc_scan_buffer = MEMAllocFromFrmHeapEx(
            mNativeWindow.heap_handle_Fg,
            drc_scan_buffer_size,
            GX2_SCAN_BUFFER_ALIGNMENT // Required alignment
        );

        if (!mNativeWindow.p_drc_scan_buffer)
            return false;

        // Flush allocated buffer from CPU cache
        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.p_drc_scan_buffer, drc_scan_buffer_size);

        // Set the current DRC scan buffer
        GX2SetDRCBuffer(
            mNativeWindow.p_drc_scan_buffer,        // Scan Buffer
            drc_scan_buffer_size,                   // Scan Buffer Size
            GX2_DRC_RENDER_MODE_SINGLE,             // Render Mode
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,   // Scan Buffer Surface Format
            GX2_BUFFERING_MODE_DOUBLE               // Enable double-buffering
        );

        // Set the current DRC scan buffer dimensions
        GX2SetDRCScale(drc_width, drc_height);
    }

    // Initialize color buffer
    mNativeWindow.color_buffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    mNativeWindow.color_buffer.surface.width = mWidth;
    mNativeWindow.color_buffer.surface.height = mHeight;
    mNativeWindow.color_buffer.surface.depth = 1;
    mNativeWindow.color_buffer.surface.mipLevels = 1;
    mNativeWindow.color_buffer.surface.format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
    mNativeWindow.color_buffer.surface.aa = GX2_AA_MODE1X;
    mNativeWindow.color_buffer.surface.use = GX2_SURFACE_USE_TEXTURE_COLOR_BUFFER_TV;
    mNativeWindow.color_buffer.surface.mipmaps = nullptr;
    mNativeWindow.color_buffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    mNativeWindow.color_buffer.surface.swizzle  = 0;
    mNativeWindow.color_buffer.viewMip = 0;
    mNativeWindow.color_buffer.viewFirstSlice = 0;
    mNativeWindow.color_buffer.viewNumSlices = 1;
    GX2CalcSurfaceSizeAndAlignment(&mNativeWindow.color_buffer.surface);
    GX2InitColorBufferRegs(&mNativeWindow.color_buffer);

    // Allocate color buffer data
    mNativeWindow.p_color_buffer_image_data = MEMAllocFromFrmHeapEx(
        mNativeWindow.heap_handle_MEM1,
        mNativeWindow.color_buffer.surface.imageSize, // Data byte size
        mNativeWindow.color_buffer.surface.alignment  // Required alignment
    );

    if (!mNativeWindow.p_color_buffer_image_data)
        return false;

    mNativeWindow.color_buffer.surface.image = mNativeWindow.p_color_buffer_image_data;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.p_color_buffer_image_data, mNativeWindow.color_buffer.surface.imageSize);

    // Copy color buffer to a texture
    mNativeWindow.color_buffer_texture.surface = mNativeWindow.color_buffer.surface;
    mNativeWindow.color_buffer_texture.surface.use = GX2_SURFACE_USE_TEXTURE;
    mNativeWindow.color_buffer_texture.viewFirstMip = 0;
    mNativeWindow.color_buffer_texture.viewNumMips = 1;
    mNativeWindow.color_buffer_texture.viewFirstSlice = 0;
    mNativeWindow.color_buffer_texture.viewNumSlices = 1;
    mNativeWindow.color_buffer_texture.compMap = 0x00010203;
    GX2InitTextureRegs(&mNativeWindow.color_buffer_texture);

    // Initialize depth buffer
    mNativeWindow.depth_buffer.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    mNativeWindow.depth_buffer.surface.width = mWidth;
    mNativeWindow.depth_buffer.surface.height = mHeight;
    mNativeWindow.depth_buffer.surface.depth = 1;
    mNativeWindow.depth_buffer.surface.mipLevels = 1;
    mNativeWindow.depth_buffer.surface.format = GX2_SURFACE_FORMAT_FLOAT_D24_S8;
    mNativeWindow.depth_buffer.surface.aa = GX2_AA_MODE1X;
    mNativeWindow.depth_buffer.surface.use = GX2_SURFACE_USE_TEXTURE | GX2_SURFACE_USE_DEPTH_BUFFER;
    mNativeWindow.depth_buffer.surface.mipmaps = nullptr;
    mNativeWindow.depth_buffer.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    mNativeWindow.depth_buffer.surface.swizzle  = 0;
    mNativeWindow.depth_buffer.viewMip = 0;
    mNativeWindow.depth_buffer.viewFirstSlice = 0;
    mNativeWindow.depth_buffer.viewNumSlices = 1;
    mNativeWindow.depth_buffer.hiZPtr = nullptr;
    mNativeWindow.depth_buffer.hiZSize = 0;
    mNativeWindow.depth_buffer.depthClear = 1.0f;
    mNativeWindow.depth_buffer.stencilClear = 0;
    GX2CalcSurfaceSizeAndAlignment(&mNativeWindow.depth_buffer.surface);
    GX2InitDepthBufferRegs(&mNativeWindow.depth_buffer);

    // Allocate depth buffer data
    mNativeWindow.p_depth_buffer_image_data = MEMAllocFromFrmHeapEx(
        mNativeWindow.heap_handle_MEM1,
        mNativeWindow.depth_buffer.surface.imageSize, // Data byte size
        mNativeWindow.depth_buffer.surface.alignment  // Required alignment
    );

    if (!mNativeWindow.p_depth_buffer_image_data)
        return false;

    mNativeWindow.depth_buffer.surface.image = mNativeWindow.p_depth_buffer_image_data;

    // Flush allocated buffer from CPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, mNativeWindow.p_depth_buffer_image_data, mNativeWindow.depth_buffer.surface.imageSize);

    // Enable TV and DRC
    GX2SetTVEnable(true);
    GX2SetDRCEnable(true);

    // If not first time in foreground, restore the GX2 context state
    if (mNativeWindow.is_running)
    {
        GX2SetContextState(mNativeWindow.p_context_state);
        GX2SetColorBuffer(&mNativeWindow.color_buffer, GX2_RENDER_TARGET_0);
        GX2SetDepthBuffer(&mNativeWindow.depth_buffer);
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
    if (mNativeWindow.is_running)
        ProcUIDrawDoneRelease();

    if (mNativeWindow.heap_handle_Fg)
    {
        MEMFreeToFrmHeap(mNativeWindow.heap_handle_Fg, MEM_FRM_HEAP_FREE_ALL);
        mNativeWindow.heap_handle_Fg = nullptr;
    }
    mNativeWindow.p_tv_scan_buffer = nullptr;
    mNativeWindow.p_drc_scan_buffer = nullptr;

    if (mNativeWindow.heap_handle_MEM1)
    {
        MEMFreeToFrmHeap(mNativeWindow.heap_handle_MEM1, MEM_FRM_HEAP_FREE_ALL);
        mNativeWindow.heap_handle_MEM1 = nullptr;
    }
    mNativeWindow.p_color_buffer_image_data = nullptr;
    mNativeWindow.p_depth_buffer_image_data = nullptr;

#ifdef RIO_DEBUG
    WHBLogCafeDeinit();
    WHBLogUdpDeinit();
#endif // RIO_DEBUG
}

bool Window::initialize_()
{
    // Allocate GX2 command buffer
    mNativeWindow.p_cmd_list = MEMAllocFromDefaultHeapEx(
        0x400000,                    // A very commonly used size in Nintendo games
        GX2_COMMAND_BUFFER_ALIGNMENT // Required alignment
    );

    if (!mNativeWindow.p_cmd_list)
    {
        terminate_();
        return false;
    }

    // Several parameters to initialize GX2 with
    u32 initAttribs[] = {
        GX2_INIT_CMD_BUF_BASE, (uintptr_t)mNativeWindow.p_cmd_list, // Command Buffer Base Address
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
    mNativeWindow.p_context_state = (GX2ContextState*)MEMAllocFromDefaultHeapEx(
        sizeof(GX2ContextState),    // Size of context
        GX2_CONTEXT_STATE_ALIGNMENT // Required alignment
    );

    if (!mNativeWindow.p_context_state)
    {
        terminate_();
        return false;
    }

    // Initialize it to default state
    GX2SetupContextStateEx(mNativeWindow.p_context_state, false);

    // Make context of window current
    makeContextCurrent();

    // Set swap interval to 1 by default
    setSwapInterval(1);

    // Scissor test is always enabled in GX2

    // Initialize ProcUI
    ProcUIInit(&OSSavesDone_ReadyToRelease);

    mNativeWindow.is_running = true;
    return true;
}

bool Window::isRunning() const
{
    return mNativeWindow.is_running;
}

void Window::terminate_()
{
    mNativeWindow.is_running = false;

    if (mNativeWindow.p_cmd_list)
    {
        MEMFreeToDefaultHeap(mNativeWindow.p_cmd_list);
        mNativeWindow.p_cmd_list = nullptr;
    }

    if (mNativeWindow.p_context_state)
    {
        MEMFreeToDefaultHeap(mNativeWindow.p_context_state);
        mNativeWindow.p_context_state = nullptr;
    }

    AudioMgr::destroySingleton();

    foregroundRelease_();

    OSBlockThreadsOnExit();
    _Exit(-1);
}

void Window::makeContextCurrent() const
{
    GX2SetContextState(mNativeWindow.p_context_state);
    GX2SetColorBuffer(&mNativeWindow.color_buffer, GX2_RENDER_TARGET_0);
    GX2SetDepthBuffer(&mNativeWindow.depth_buffer);
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
    GX2CopyColorBufferToScanBuffer(&mNativeWindow.color_buffer, GX2_SCAN_TARGET_TV);
    GX2CopyColorBufferToScanBuffer(&mNativeWindow.color_buffer, GX2_SCAN_TARGET_DRC);
    // Flip
    GX2SwapScanBuffers();

    // Reset context state for next frame
    GX2SetContextState(mNativeWindow.p_context_state);

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
    GX2ClearColor(&mNativeWindow.color_buffer, r, g, b, a);

    // GX2ClearColor invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearDepth()
{
    // Clear
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.depth_buffer,
                           mNativeWindow.depth_buffer.depthClear,
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
    GX2ClearDepthStencilEx(&mNativeWindow.depth_buffer,
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
    GX2ClearDepthStencilEx(&mNativeWindow.depth_buffer,
                           0,
                           mNativeWindow.depth_buffer.stencilClear,
                           GX2_CLEAR_FLAGS_STENCIL);

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

void Window::clearStencil(u8 stencil)
{
    // Clear using the given stencil
    // Does not need a current context to be set
    GX2ClearDepthStencilEx(&mNativeWindow.depth_buffer,
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
    GX2ClearDepthStencilEx(&mNativeWindow.depth_buffer,
                           mNativeWindow.depth_buffer.depthClear,
                           mNativeWindow.depth_buffer.stencilClear,
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
    GX2ClearDepthStencilEx(&mNativeWindow.depth_buffer,
                           depth,
                           stencil,
                           (GX2ClearFlags)(GX2_CLEAR_FLAGS_DEPTH |
                                           GX2_CLEAR_FLAGS_STENCIL));

    // GX2ClearDepthStencilEx invalidates the current context and the window context
    // must be made current again
    makeContextCurrent();
}

}

#endif // RIO_IS_CAFE
