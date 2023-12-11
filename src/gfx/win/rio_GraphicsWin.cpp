#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gfx/rio_Graphics.h>

#ifdef RIO_WIN_GL_SCISSOR_INVERTED
#include <gfx/rio_Window.h>
#endif // RIO_WIN_GL_SCISSOR_INVERTED

namespace rio {

s32 Graphics::sViewportX;
s32 Graphics::sViewportY;
u32 Graphics::sViewportWidth;
u32 Graphics::sViewportHeight;
f32 Graphics::sViewportNear;
f32 Graphics::sViewportFar;
s32 Graphics::sScissorX;
s32 Graphics::sScissorY;
u32 Graphics::sScissorWidth;
u32 Graphics::sScissorHeight;

void Graphics::setViewport(s32 x, s32 y, u32 width, u32 height, f32 near, f32 far, s32 frame_buffer_height)
{
#ifdef RIO_WIN_GL_SCISSOR_INVERTED
    y = (frame_buffer_height == -1 ? Window::instance()->getHeight() : frame_buffer_height) - y - height;
#endif // RIO_WIN_GL_SCISSOR_INVERTED

    sViewportX = x;
    sViewportY = y;
    sViewportWidth = width;
    sViewportHeight = height;
    sViewportNear = near;
    sViewportFar = far;

    RIO_GL_CALL(glViewport(x, y, width, height));
    RIO_GL_CALL(glDepthRange(near, far));
}

void Graphics::setScissor(s32 x, s32 y, u32 width, u32 height, s32 frame_buffer_height)
{
#ifdef RIO_WIN_GL_SCISSOR_INVERTED
    y = (frame_buffer_height == -1 ? Window::instance()->getHeight() : frame_buffer_height) - y - height;
#endif // RIO_WIN_GL_SCISSOR_INVERTED

    sScissorX = x;
    sScissorY = y;
    sScissorWidth = width;
    sScissorHeight = height;

    RIO_GL_CALL(glScissor(x, y, width, height));
}

}

#endif // RIO_IS_WIN
