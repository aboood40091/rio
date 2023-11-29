#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_RenderStateMRT.h>

namespace rio {

void RenderStateMRT::apply() const
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

    for (u32 target = 0; target < Graphics::RENDER_TARGET_MAX_NUM; target++)
    {
        RIO_GL_CALL((mBlendEnableMask.isOnBit(target) ? glEnablei : glDisablei)(GL_BLEND, target));
        RIO_GL_CALL(glBlendFuncSeparatei(target,
                                         mBlendExpression[target].blend_factor_src_rgb,
                                         mBlendExpression[target].blend_factor_dst_rgb,
                                         mBlendExpression[target].blend_factor_src_a,
                                         mBlendExpression[target].blend_factor_dst_a));
        RIO_GL_CALL(glBlendEquationSeparatei(target,
                                             mBlendExpression[target].blend_equation_rgb,
                                             mBlendExpression[target].blend_equation_a));
    }

    RIO_GL_CALL(glBlendColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b, mBlendConstantColor.a));

    for (u32 target = 0; target < Graphics::RENDER_TARGET_MAX_NUM; target++)
    {
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        RIO_GL_CALL(glColorMaski(target,
                                 color_mask >> 0 & 1,
                                 color_mask >> 1 & 1,
                                 color_mask >> 2 & 1,
                                 color_mask >> 3 & 1));
    }

    RIO_GL_CALL(glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE));
}

void RenderStateMRT::applyDepthAndStencilTest() const
{
    RIO_GL_CALL((mDepthTestEnable ? glEnable : glDisable)(GL_DEPTH_TEST));
    RIO_GL_CALL(glDepthMask(mDepthWriteEnable ? GL_TRUE : GL_FALSE));
    RIO_GL_CALL(glDepthFunc(mDepthFunc));

    RIO_GL_CALL((mStencilTestEnable ? glEnable : glDisable)(GL_STENCIL_TEST));
    RIO_GL_CALL(glStencilFunc(mStencilTestFunc, mStencilTestRef, mStencilTestMask));
    RIO_GL_CALL(glStencilOp(mStencilOpFail, mStencilOpZFail, mStencilOpZPass));
}

void RenderStateMRT::applyColorMask() const
{
    for (u32 target = 0; target < Graphics::RENDER_TARGET_MAX_NUM; target++)
    {
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        RIO_GL_CALL(glColorMaski(target,
                                 color_mask >> 0 & 1,
                                 color_mask >> 1 & 1,
                                 color_mask >> 2 & 1,
                                 color_mask >> 3 & 1));
    }
}

void RenderStateMRT::applyBlendAndFastZ() const
{
    for (u32 target = 0; target < Graphics::RENDER_TARGET_MAX_NUM; target++)
    {
        RIO_GL_CALL((mBlendEnableMask.isOnBit(target) ? glEnablei : glDisablei)(GL_BLEND, target));
        RIO_GL_CALL(glBlendFuncSeparatei(target,
                                         mBlendExpression[target].blend_factor_src_rgb,
                                         mBlendExpression[target].blend_factor_dst_rgb,
                                         mBlendExpression[target].blend_factor_src_a,
                                         mBlendExpression[target].blend_factor_dst_a));
        RIO_GL_CALL(glBlendEquationSeparatei(target,
                                             mBlendExpression[target].blend_equation_rgb,
                                             mBlendExpression[target].blend_equation_a));
    }
}

void RenderStateMRT::applyBlendConstantColor() const
{
    RIO_GL_CALL(glBlendColor(mBlendConstantColor.r, mBlendConstantColor.g, mBlendConstantColor.b, mBlendConstantColor.a));
}

void RenderStateMRT::applyCullingAndPolygonModeAndPolygonOffset() const
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
