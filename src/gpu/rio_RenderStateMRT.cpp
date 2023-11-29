#include <gpu/rio_RenderStateMRT.h>

namespace rio {

RenderStateMRT::RenderStateMRT()
    : mDepthTestEnable(true)
    , mDepthWriteEnable(true)
    , mDepthFunc(Graphics::COMPARE_FUNC_LEQUAL)

    , mCullingMode(Graphics::CULLING_MODE_BACK)

    , mBlendEnableMask(0xFF)
    , mBlendConstantColor{ 1.0f, 1.0f, 1.0f, 1.0f }

    , mColorMask(0xFFFFFFFF)

    , mStencilTestEnable(false)
    , mStencilTestFunc(Graphics::COMPARE_FUNC_NEVER)
    , mStencilTestRef(0)
    , mStencilTestMask(0xFFFFFFFF)
    , mStencilOpFail(Graphics::STENCIL_KEEP)
    , mStencilOpZFail(Graphics::STENCIL_KEEP)
    , mStencilOpZPass(Graphics::STENCIL_KEEP)

    , mPolygonMode(Graphics::POLYGON_MODE_FILL)
    , mPolygonOffsetEnable(false)
    , mPolygonOffsetPointLineEnable(false)
{
}

}
