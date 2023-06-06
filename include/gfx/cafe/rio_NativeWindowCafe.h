#ifndef RIO_GFX_NATIVE_WINDOW_CAFE_H
#define RIO_GFX_NATIVE_WINDOW_CAFE_H

#include <misc/rio_Types.h>

#include <coreinit/memheap.h>
#include <gx2/context.h>
#include <gx2/texture.h>

namespace rio {

struct NativeWindow
{
    NativeWindow()
        : p_cmd_list(nullptr)
        , p_context_state(nullptr)
        , p_tv_scan_buffer(nullptr)
        , p_drc_scan_buffer(nullptr)
        , p_color_buffer_image_data(nullptr)
        , p_depth_buffer_image_data(nullptr)
        , is_running(false)
    {
    }

    void* p_cmd_list;

    GX2ContextState* p_context_state;

    void* p_tv_scan_buffer;
    void* p_drc_scan_buffer;

    GX2ColorBuffer color_buffer;
    GX2Texture color_buffer_texture;
    void* p_color_buffer_image_data;

    GX2DepthBuffer depth_buffer;
    void* p_depth_buffer_image_data;

    MEMHeapHandle heap_handle_MEM1;
    MEMHeapHandle heap_handle_Fg;

    bool is_running;
};

}

#endif // RIO_GFX_NATIVE_WINDOW_CAFE_H
