#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_RenderState.h>

namespace rio {

void RenderState::apply() const
{
    (mDepthTestEnable ? glEnable : glDisable)(GL_DEPTH_TEST);
    glDepthMask(mDepthWriteEnable ? GL_TRUE : GL_FALSE);
    glDepthFunc(mDepthFunc);

    (mStencilTestEnable ? glEnable : glDisable)(GL_STENCIL_TEST);
    glStencilFunc(mStencilTestFunc, mStencilTestRef, mStencilTestMask);
    glStencilOp(mStencilOpFail, mStencilOpZFail, mStencilOpZPass);

    switch (mCullingMode)
    {
    case Graphics::CULLING_MODE_NONE:
        glDisable(GL_CULL_FACE);
        break;
    case Graphics::CULLING_MODE_FRONT:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
    case Graphics::CULLING_MODE_BACK:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
    case Graphics::CULLING_MODE_ALL:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);
    }

    glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode);

    (mPolygonOffsetEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_FILL);
    (mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_POINT);
    (mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_LINE);

    (mBlendEnable ? glEnable : glDisable)(GL_BLEND);
    glBlendFuncSeparate(mBlendFactorSrcRGB, mBlendFactorDstRGB, mBlendFactorSrcA, mBlendFactorDstA);
    glBlendEquationSeparate(mBlendEquationRGB, mBlendEquationA);
    glBlendColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b, mBlendConstantColor.a);

    glColorMask(mColorMaskR, mColorMaskG, mColorMaskB, mColorMaskA);

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void RenderState::applyDepthAndStencilTest() const
{
    (mDepthTestEnable ? glEnable : glDisable)(GL_DEPTH_TEST);
    glDepthMask(mDepthWriteEnable ? GL_TRUE : GL_FALSE);
    glDepthFunc(mDepthFunc);

    (mStencilTestEnable ? glEnable : glDisable)(GL_STENCIL_TEST);
    glStencilFunc(mStencilTestFunc, mStencilTestRef, mStencilTestMask);
    glStencilOp(mStencilOpFail, mStencilOpZFail, mStencilOpZPass);
}

void RenderState::applyColorMask() const
{
    glColorMask(mColorMaskR, mColorMaskG, mColorMaskB, mColorMaskA);
}

void RenderState::applyBlendAndFastZ() const
{
    (mBlendEnable ? glEnable : glDisable)(GL_BLEND);
    glBlendFuncSeparate(mBlendFactorSrcRGB, mBlendFactorDstRGB, mBlendFactorSrcA, mBlendFactorDstA);
    glBlendEquationSeparate(mBlendEquationRGB, mBlendEquationA);
}

void RenderState::applyBlendConstantColor() const
{
    glBlendColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b, mBlendConstantColor.a);
}

void RenderState::applyCullingAndPolygonModeAndPolygonOffset() const
{
    switch (mCullingMode)
    {
    case Graphics::CULLING_MODE_NONE:
        glDisable(GL_CULL_FACE);
        break;
    case Graphics::CULLING_MODE_FRONT:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
    case Graphics::CULLING_MODE_BACK:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
    case Graphics::CULLING_MODE_ALL:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);
    }

    glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode);

    (mPolygonOffsetEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_FILL);
    (mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_POINT);
    (mPolygonOffsetPointLineEnable ? glEnable : glDisable)(GL_POLYGON_OFFSET_LINE);
}

}

#endif // RIO_IS_WIN
