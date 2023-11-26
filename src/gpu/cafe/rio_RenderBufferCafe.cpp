#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_RenderBuffer.h>
#include <gpu/rio_RenderTarget.h>

#include <gx2/clear.h>

namespace rio {

void RenderBuffer::bindColorClear(f32 r, f32 g, f32 b, f32 a)
{
    RIO_ASSERT(getRenderTargetColor() != nullptr);

    // Clear using the given color
    GX2ClearColor(&getRenderTargetColor()->getNativeColorBuffer(), r, g, b, a);

    // Bind the render buffer
    bind();
}

void RenderBuffer::bindDepthClear(f32 depth)
{
    RIO_ASSERT(getRenderTargetDepth() != nullptr);

    // Clear using the given depth
    GX2ClearDepthStencilEx(&getRenderTargetDepth()->getNativeDepthBuffer(),
                           depth,
                           0,
                           GX2_CLEAR_FLAGS_DEPTH);

    // Bind the render buffer
    bind();
}

void RenderBuffer::bindStencilClear(u8 stencil)
{
    RIO_ASSERT(getRenderTargetDepth() != nullptr);

    // Clear using the given stencil
    GX2ClearDepthStencilEx(&getRenderTargetDepth()->getNativeDepthBuffer(),
                           0,
                           stencil,
                           GX2_CLEAR_FLAGS_STENCIL);

    // Bind the render buffer
    bind();
}

void RenderBuffer::bindDepthStencilClear(f32 depth, u8 stencil)
{
    RIO_ASSERT(getRenderTargetDepth() != nullptr);

    // Clear using the given depth-stencil
    GX2ClearDepthStencilEx(&getRenderTargetDepth()->getNativeDepthBuffer(),
                           depth,
                           stencil,
                           (GX2ClearFlags)(GX2_CLEAR_FLAGS_DEPTH |
                                           GX2_CLEAR_FLAGS_STENCIL));

    // Bind the render buffer
    bind();
}

}

#endif // RIO_IS_CAFE
