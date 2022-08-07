#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_RenderState.h>

#include <gx2/registers.h>

namespace rio {

void RenderState::apply() const
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
        mBlendEnable ? 0xFF : 0,
        GX2_DISABLE,
        static_cast<BOOL>(mColorMaskR || mColorMaskG || mColorMaskB || mColorMaskA)
    );

    GX2SetBlendControl(
        GX2_RENDER_TARGET_0,
        static_cast<GX2BlendMode>(mBlendFactorSrcRGB),
        static_cast<GX2BlendMode>(mBlendFactorDstRGB),
        static_cast<GX2BlendCombineMode>(mBlendEquationRGB),
        GX2_ENABLE,
        static_cast<GX2BlendMode>(mBlendFactorSrcA),
        static_cast<GX2BlendMode>(mBlendFactorDstA),
        static_cast<GX2BlendCombineMode>(mBlendEquationA)
    );

    GX2SetBlendConstantColor(mBlendConstantColor.r,
                             mBlendConstantColor.g,
                             mBlendConstantColor.b,
                             mBlendConstantColor.a);

    GX2SetAlphaTest(GX2_FALSE,
                    GX2_COMPARE_FUNC_ALWAYS,
                    0.0f);

    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(mColorMaskR * GX2_CHANNEL_MASK_R +
                                    mColorMaskG * GX2_CHANNEL_MASK_G +
                                    mColorMaskB * GX2_CHANNEL_MASK_B +
                                    mColorMaskA * GX2_CHANNEL_MASK_A),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0)
    );

    GX2SetAlphaToMask(GX2_DISABLE, GX2_ALPHA_TO_MASK_MODE_NON_DITHERED);
}

void RenderState::applyDepthAndStencilTest() const
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

void RenderState::applyColorMask() const
{
    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(mColorMaskR * GX2_CHANNEL_MASK_R +
                                    mColorMaskG * GX2_CHANNEL_MASK_G +
                                    mColorMaskB * GX2_CHANNEL_MASK_B +
                                    mColorMaskA * GX2_CHANNEL_MASK_A),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0),
        static_cast<GX2ChannelMask>(0)
    );
}

void RenderState::applyBlendAndFastZ() const
{
    GX2SetColorControl(
        GX2_LOGIC_OP_COPY,
        mBlendEnable ? 0xFF : 0,
        GX2_DISABLE,
        static_cast<BOOL>(mColorMaskR || mColorMaskG || mColorMaskB || mColorMaskA)
    );

    GX2SetBlendControl(
        GX2_RENDER_TARGET_0,
        static_cast<GX2BlendMode>(mBlendFactorSrcRGB),
        static_cast<GX2BlendMode>(mBlendFactorDstRGB),
        static_cast<GX2BlendCombineMode>(mBlendEquationRGB),
        GX2_ENABLE,
        static_cast<GX2BlendMode>(mBlendFactorSrcA),
        static_cast<GX2BlendMode>(mBlendFactorDstA),
        static_cast<GX2BlendCombineMode>(mBlendEquationA)
    );
}

void RenderState::applyBlendConstantColor() const
{
    GX2SetBlendConstantColor(mBlendConstantColor.r,
                             mBlendConstantColor.g,
                             mBlendConstantColor.b,
                             mBlendConstantColor.a);
}

void RenderState::applyCullingAndPolygonModeAndPolygonOffset() const
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
