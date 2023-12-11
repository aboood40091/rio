#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gfx/rio_Graphics.h>

#include <gx2/registers.h>

namespace rio {

void Graphics::setViewport(s32 x, s32 y, u32 width, u32 height, f32 near, f32 far, s32)
{
    GX2SetViewport(x, y, width, height, near, far);
}

void Graphics::setScissor(s32 x, s32 y, u32 width, u32 height, s32)
{
    GX2SetScissor(x, y, width, height);
}

}

#endif // RIO_IS_CAFE
