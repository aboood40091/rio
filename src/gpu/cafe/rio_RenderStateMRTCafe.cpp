#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_RenderStateMRT.h>

#include <gx2/registers.h>

namespace rio {

void RenderStateMRT::apply() const
{
    GX2SetDepthStencilControl(
        static_cast<BOOL>(mDepthTestEnable),
        static_cast<BOOL>(mDepthWriteEnable),
        static_cast<GX2CompareFunction>(mDepthFunc),
        static_cast<BOOL>(mStencilTestEnable),
        static_cast<BOOL>(mStencilTestEnable),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail)
    );

    GX2SetStencilMask(mStencilTestMask, mStencilTestMask, mStencilTestRef,
                      mStencilTestMask, mStencilTestMask, mStencilTestRef);

    {
        static const BOOL cCullSettings[4][2] = {
            { GX2_ENABLE, GX2_DISABLE },
            { GX2_DISABLE, GX2_ENABLE },
            { GX2_DISABLE, GX2_DISABLE },
            { GX2_ENABLE, GX2_ENABLE }
        };
        const BOOL* const& settings = cCullSettings[mCullingMode];

        GX2SetPolygonControl(
            GX2_FRONT_FACE_CCW,
            settings[0],
            settings[1],
            static_cast<BOOL>(mPolygonMode != Graphics::POLYGON_MODE_FILL),
            static_cast<GX2PolygonMode>(mPolygonMode),
            static_cast<GX2PolygonMode>(mPolygonMode),
            static_cast<BOOL>(mPolygonOffsetEnable),
            static_cast<BOOL>(mPolygonOffsetEnable),
            static_cast<BOOL>(mPolygonOffsetPointLineEnable)
        );
    }

    GX2SetColorControl(
        GX2_LOGIC_OP_COPY,
        mBlendEnableMask.getDirect() & 0xFF,
        GX2_DISABLE,
        static_cast<BOOL>(mColorMask != 0)
    );

    for (u32 target = 0; target < Graphics::RENDER_TARGET_MAX_NUM; target++)
    {
        GX2SetBlendControl(
            static_cast<GX2RenderTarget>(GX2_RENDER_TARGET_0 + target),
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_src_rgb),
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_dst_rgb),
            static_cast<GX2BlendCombineMode>(mBlendExpression[target].blend_equation_rgb),
            GX2_ENABLE,
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_src_a),
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_dst_a),
            static_cast<GX2BlendCombineMode>(mBlendExpression[target].blend_equation_a)
        );
    }

    GX2SetBlendConstantColor(mBlendConstantColor.r,
                             mBlendConstantColor.g,
                             mBlendConstantColor.b,
                             mBlendConstantColor.a);

    GX2SetAlphaTest(GX2_FALSE,
                    GX2_COMPARE_FUNC_ALWAYS,
                    0.0f);

    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(mColorMask >>  0 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  4 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  8 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 12 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 16 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 20 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 24 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 28 & 0xF)
    );

    GX2SetAlphaToMask(GX2_DISABLE, GX2_ALPHA_TO_MASK_MODE_NON_DITHERED);
}

void RenderStateMRT::applyDepthAndStencilTest() const
{
    GX2SetDepthStencilControl(
        static_cast<BOOL>(mDepthTestEnable),
        static_cast<BOOL>(mDepthWriteEnable),
        static_cast<GX2CompareFunction>(mDepthFunc),
        static_cast<BOOL>(mStencilTestEnable),
        static_cast<BOOL>(mStencilTestEnable),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail)
    );

    GX2SetStencilMask(mStencilTestMask, mStencilTestMask, mStencilTestRef,
                      mStencilTestMask, mStencilTestMask, mStencilTestRef);
}

void RenderStateMRT::applyColorMask() const
{
    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(mColorMask >>  0 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  4 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  8 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 12 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 16 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 20 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 24 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 28 & 0xF)
    );
}

void RenderStateMRT::applyBlendAndFastZ() const
{
    GX2SetColorControl(
        GX2_LOGIC_OP_COPY,
        mBlendEnableMask.getDirect() & 0xFF,
        GX2_DISABLE,
        static_cast<BOOL>(mColorMask != 0)
    );

    for (u32 target = 0; target < Graphics::RENDER_TARGET_MAX_NUM; target++)
    {
        GX2SetBlendControl(
            static_cast<GX2RenderTarget>(GX2_RENDER_TARGET_0 + target),
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_src_rgb),
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_dst_rgb),
            static_cast<GX2BlendCombineMode>(mBlendExpression[target].blend_equation_rgb),
            GX2_ENABLE,
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_src_a),
            static_cast<GX2BlendMode>(mBlendExpression[target].blend_factor_dst_a),
            static_cast<GX2BlendCombineMode>(mBlendExpression[target].blend_equation_a)
        );
    }
}

void RenderStateMRT::applyBlendConstantColor() const
{
    GX2SetBlendConstantColor(mBlendConstantColor.r,
                             mBlendConstantColor.g,
                             mBlendConstantColor.b,
                             mBlendConstantColor.a);
}

void RenderStateMRT::applyCullingAndPolygonModeAndPolygonOffset() const
{
    static const BOOL cCullSettings[4][2] = {
        { GX2_ENABLE, GX2_DISABLE },
        { GX2_DISABLE, GX2_ENABLE },
        { GX2_DISABLE, GX2_DISABLE },
        { GX2_ENABLE, GX2_ENABLE }
    };
    const BOOL* const& settings = cCullSettings[mCullingMode];

    GX2SetPolygonControl(
        GX2_FRONT_FACE_CCW,
        settings[0],
        settings[1],
        static_cast<BOOL>(mPolygonMode != Graphics::POLYGON_MODE_FILL),
        static_cast<GX2PolygonMode>(mPolygonMode),
        static_cast<GX2PolygonMode>(mPolygonMode),
        static_cast<BOOL>(mPolygonOffsetEnable),
        static_cast<BOOL>(mPolygonOffsetEnable),
        static_cast<BOOL>(mPolygonOffsetPointLineEnable)
    );
}

}

#endif // RIO_IS_CAFE
