#ifndef RIO_GFX_NATIVE_WINDOW_WIN_H
#define RIO_GFX_NATIVE_WINDOW_WIN_H

#include <misc/gl/rio_GL.h>

namespace rio {

struct NativeWindow
{
    NativeWindow()
        : handle(nullptr)
        , frame_buffer_handle(GL_NONE)
        , color_buffer_handle(GL_NONE)
        , depth_buffer_handle(GL_NONE)
    {
    }

    GLFWwindow* handle;

    GLuint frame_buffer_handle;
    GLuint color_buffer_handle;
    GLuint depth_buffer_handle;
};

}

#endif // RIO_GFX_NATIVE_WINDOW_WIN_H
