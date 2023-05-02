#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_RenderState.h>

namespace rio {

void RenderState::apply() const
{
    RIO_GL_CALL((mDepthTestEnable ? glEnable : glDisable)(GL_DEPTH_TEST));
    RIO_GL_CALL(glDepthMask(mDepthWriteEnable ? GL_TRUE : GL_FALSE));
    RIO_GL_CALL(glDepthFunc(mDepthFunc));

    RIO_GL_CALL((mStencilTestEnable ? glEnable : glDisable)(GL_STENCIL_TEST));
    RIO_GL_CALL(glStencilFunc(mStencilTestFunc, mStencilTestRef, mStencilTestMask));
    RIO_GL_CALL(glStencilOp(mStencilOpFail, mStencilOpZFail, mStencilOpZPass));

    switch (mCullingMode)
    {
    case Graphics::CULLING_MODE_NONE:
        RIO_GL_CALL(glDisable(GL_CULL_FACE));
        break;
    case Graphics::CULLING_MODE_FRONT:
        RIO_GL_CALL(glEnable(GL_CULL_FACE));
        RIO_GL_CALL(glCullFace(GL_FRONT));
        break;
    case Graphics::CULLING_MODE_BACK:
        RIO_GL_CALL(glEnable(GL_CULL_FACE));
        RIO_GL_CALL(glCullFace(GL_BACK));
        break;
    case Graphics::CULLING_MODE_ALL:
        RIO_GL_CALL(glEnable(GL_CULL_FACE));
        RIO_GL_CALL(glCullFace(GL_FRONT_AND_BACK));
    }

    RIO_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode));

    RIO_GL_CALL((mPolygonOffsetEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_FILL));
    RIO_GL_CALL((mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_POINT));
    RIO_GL_CALL((mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_LINE));

    RIO_GL_CALL((mBlendEnable ? glEnable : glDisable)(GL_BLEND));
    RIO_GL_CALL(glBlendFuncSeparate(mBlendFactorSrcRGB, mBlendFactorDstRGB, mBlendFactorSrcA, mBlendFactorDstA));
    RIO_GL_CALL(glBlendEquationSeparate(mBlendEquationRGB, mBlendEquationA));
    RIO_GL_CALL(glBlendColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b, mBlendConstantColor.a));

    RIO_GL_CALL(glColorMask(mColorMaskR, mColorMaskG, mColorMaskB, mColorMaskA));

    RIO_GL_CALL(glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE));
}

void RenderState::applyDepthAndStencilTest() const
{
    RIO_GL_CALL((mDepthTestEnable ? glEnable : glDisable)(GL_DEPTH_TEST));
    RIO_GL_CALL(glDepthMask(mDepthWriteEnable ? GL_TRUE : GL_FALSE));
    RIO_GL_CALL(glDepthFunc(mDepthFunc));

    RIO_GL_CALL((mStencilTestEnable ? glEnable : glDisable)(GL_STENCIL_TEST));
    RIO_GL_CALL(glStencilFunc(mStencilTestFunc, mStencilTestRef, mStencilTestMask));
    RIO_GL_CALL(glStencilOp(mStencilOpFail, mStencilOpZFail, mStencilOpZPass));
}

void RenderState::applyColorMask() const
{
    RIO_GL_CALL(glColorMask(mColorMaskR, mColorMaskG, mColorMaskB, mColorMaskA));
}

void RenderState::applyBlendAndFastZ() const
{
    RIO_GL_CALL((mBlendEnable ? glEnable : glDisable)(GL_BLEND));
    RIO_GL_CALL(glBlendFuncSeparate(mBlendFactorSrcRGB, mBlendFactorDstRGB, mBlendFactorSrcA, mBlendFactorDstA));
    RIO_GL_CALL(glBlendEquationSeparate(mBlendEquationRGB, mBlendEquationA));
}

void RenderState::applyBlendConstantColor() const
{
    RIO_GL_CALL(glBlendColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b, mBlendConstantColor.a));
}

void RenderState::applyCullingAndPolygonModeAndPolygonOffset() const
{
    switch (mCullingMode)
    {
    case Graphics::CULLING_MODE_NONE:
        RIO_GL_CALL(glDisable(GL_CULL_FACE));
        break;
    case Graphics::CULLING_MODE_FRONT:
        RIO_GL_CALL(glEnable(GL_CULL_FACE));
        RIO_GL_CALL(glCullFace(GL_FRONT));
        break;
    case Graphics::CULLING_MODE_BACK:
        RIO_GL_CALL(glEnable(GL_CULL_FACE));
        RIO_GL_CALL(glCullFace(GL_BACK));
        break;
    case Graphics::CULLING_MODE_ALL:
        RIO_GL_CALL(glEnable(GL_CULL_FACE));
        RIO_GL_CALL(glCullFace(GL_FRONT_AND_BACK));
    }

    RIO_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode));

    RIO_GL_CALL((mPolygonOffsetEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_FILL));
    RIO_GL_CALL((mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_POINT));
    RIO_GL_CALL((mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_LINE));
}

}

#endif // RIO_IS_WIN
