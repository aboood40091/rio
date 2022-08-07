#include <gpu/rio_RenderState.h>

namespace rio {

RenderState::RenderState()
    : mDepthTestEnable(true)
    , mDepthWriteEnable(true)
    , mDepthFunc(Graphics::COMPARE_FUNC_LEQUAL)

    , mCullingMode(Graphics::CULLING_MODE_BACK)

    , mBlendEnable(true)
    , mBlendFactorSrcRGB(Graphics::BLEND_MODE_SRC_ALPHA)
    , mBlendFactorSrcA(Graphics::BLEND_MODE_SRC_ALPHA)
    , mBlendFactorDstRGB(Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA)
    , mBlendFactorDstA(Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA)
    , mBlendEquationRGB(Graphics::BLEND_FUNC_ADD)
    , mBlendEquationA(Graphics::BLEND_FUNC_ADD)
    , mBlendConstantColor{ 1.0f, 1.0f, 1.0f, 1.0f }

    , mColorMaskR(true)
    , mColorMaskG(true)
    , mColorMaskB(true)
    , mColorMaskA(true)

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
