#ifndef RIO_GPU_RENDER_STATE_MRT_H
#define RIO_GPU_RENDER_STATE_MRT_H

#include <gfx/rio_Color.h>
#include <gfx/rio_Graphics.h>
#include <misc/rio_BitFlag.h>

namespace rio {

class RenderStateMRT
{
    struct BlendExpression
    {
        BlendExpression()
            : blend_factor_src_rgb(Graphics::BLEND_MODE_SRC_ALPHA)
            , blend_factor_src_a(Graphics::BLEND_MODE_SRC_ALPHA)
            , blend_factor_dst_rgb(Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA)
            , blend_factor_dst_a(Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA)
            , blend_equation_rgb(Graphics::BLEND_FUNC_ADD)
            , blend_equation_a(Graphics::BLEND_FUNC_ADD)
        {
        }

        Graphics::BlendFactor   blend_factor_src_rgb;
        Graphics::BlendFactor   blend_factor_src_a;
        Graphics::BlendFactor   blend_factor_dst_rgb;
        Graphics::BlendFactor   blend_factor_dst_a;
        Graphics::BlendEquation blend_equation_rgb;
        Graphics::BlendEquation blend_equation_a;
    };

public:
    RenderStateMRT();

    void apply() const;
    void applyDepthAndStencilTest() const;
    void applyColorMask() const;
    void applyBlendAndFastZ() const;
    void applyBlendConstantColor() const;
    void applyCullingAndPolygonModeAndPolygonOffset() const;

    void setDepthEnable(bool test_enable, bool write_enable)
    {
        setDepthTestEnable(test_enable);
        setDepthWriteEnable(write_enable);
    }

    void setDepthTestEnable(bool test_enable)
    {
        mDepthTestEnable = test_enable;
    }

    void setDepthWriteEnable(bool write_enable)
    {
        mDepthWriteEnable = write_enable;
    }

    void setDepthFunc(Graphics::CompareFunc func)
    {
        mDepthFunc = func;
    }

    void setCullingMode(Graphics::CullingMode mode)
    {
        mCullingMode = mode;
    }

    void setBlendEnable(u32 target, bool blend)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendEnableMask.changeBit(target, blend);
    }

    void setBlendFactor(u32 target, Graphics::BlendFactor src_factor, Graphics::BlendFactor dst_factor)
    {
        setBlendFactorSrc(target, src_factor);
        setBlendFactorDst(target, dst_factor);
    }

    void setBlendFactorSeparate(u32 target, Graphics::BlendFactor src_factor_rgb, Graphics::BlendFactor dst_factor_rgb, Graphics::BlendFactor src_factor_a, Graphics::BlendFactor dst_factor_a)
    {
        setBlendFactorSrcRGB(target, src_factor_rgb);
        setBlendFactorDstRGB(target, dst_factor_rgb);
        setBlendFactorSrcAlpha(target, src_factor_a);
        setBlendFactorDstAlpha(target, dst_factor_a);
    }

    void setBlendFactorSrc(u32 target, Graphics::BlendFactor factor)
    {
        setBlendFactorSrcRGB(target, factor);
        setBlendFactorSrcAlpha(target, factor);
    }

    void setBlendFactorDst(u32 target, Graphics::BlendFactor factor)
    {
        setBlendFactorDstRGB(target, factor);
        setBlendFactorDstAlpha(target, factor);
    }

    void setBlendFactorSrcRGB(u32 target, Graphics::BlendFactor factor)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendExpression[target].blend_factor_src_rgb = factor;
    }

    void setBlendFactorSrcAlpha(u32 target, Graphics::BlendFactor factor)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendExpression[target].blend_factor_src_a = factor;
    }

    void setBlendFactorDstRGB(u32 target, Graphics::BlendFactor factor)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendExpression[target].blend_factor_dst_rgb = factor;
    }

    void setBlendFactorDstAlpha(u32 target, Graphics::BlendFactor factor)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendExpression[target].blend_factor_dst_a = factor;
    }

    void setBlendEquation(u32 target, Graphics::BlendEquation equation)
    {
        setBlendEquationRGB(target, equation);
        setBlendEquationAlpha(target, equation);
    }

    void setBlendEquationSeparate(u32 target, Graphics::BlendEquation equation_rgb, Graphics::BlendEquation equation_a)
    {
        setBlendEquationRGB(target, equation_rgb);
        setBlendEquationAlpha(target, equation_a);
    }

    void setBlendEquationRGB(u32 target, Graphics::BlendEquation equation)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendExpression[target].blend_equation_rgb = equation;
    }

    void setBlendEquationAlpha(u32 target, Graphics::BlendEquation equation)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        mBlendExpression[target].blend_equation_a = equation;
    }

    void setBlendConstantColor(const Color4f& color)
    {
        mBlendConstantColor = color;
    }

    void setColorMask(u32 target, bool r, bool g, bool b, bool a)
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        u32 color_mask = r << 0 |
                         g << 1 |
                         b << 2 |
                         a << 3;
        mColorMask = (mColorMask & ~(0xF << (target * 4))) | (color_mask << (target * 4));
    }

    void setStencilTestEnable(bool enable)
    {
        mStencilTestEnable = enable;
    }

    void setStencilTestFunc(Graphics::CompareFunc func, s32 ref, u32 mask)
    {
        mStencilTestFunc = func;
        mStencilTestRef = ref;
        mStencilTestMask = mask;
    }

    void setStencilTestOp(Graphics::StencilOp fail, Graphics::StencilOp zfail, Graphics::StencilOp zpass)
    {
        mStencilOpFail = fail;
        mStencilOpZFail = zfail;
        mStencilOpZPass = zpass;
    }

    void setPolygonMode(Graphics::PolygonMode mode)
    {
        mPolygonMode = mode;
    }

    void setPolygonOffsetEnable(bool fill_enable, bool point_line_enable)
    {
        mPolygonOffsetEnable = fill_enable;
        mPolygonOffsetPointLineEnable = point_line_enable;
    }

    bool getDepthTestEnable() const
    {
        return mDepthTestEnable;
    }

    bool getDepthWriteEnable() const
    {
        return mDepthWriteEnable;
    }

    Graphics::CompareFunc getDepthFunc() const
    {
        return mDepthFunc;
    }

    Graphics::CullingMode getCullingMode() const
    {
        return mCullingMode;
    }

    bool getBlendEnable(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendEnableMask.isOnBit(target);
    }

    Graphics::BlendFactor getBlendFactorSrcRGB(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendExpression[target].blend_factor_src_rgb;
    }

    Graphics::BlendFactor getBlendFactorSrcAlpha(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendExpression[target].blend_factor_src_a;
    }

    Graphics::BlendFactor getBlendFactorDstRGB(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendExpression[target].blend_factor_dst_rgb;
    }

    Graphics::BlendFactor getBlendFactorDstAlpha(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendExpression[target].blend_factor_dst_a;
    }

    Graphics::BlendEquation getBlendEquationRGB(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendExpression[target].blend_equation_rgb;
    }

    Graphics::BlendEquation getBlendEquationAlpha(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        return mBlendExpression[target].blend_equation_a;
    }

    const Color4f& getBlendConstantColor() const
    {
        return mBlendConstantColor;
    }

    bool getColorMaskR(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 0 & 1;
    }

    bool getColorMaskG(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 1 & 1;
    }

    bool getColorMaskB(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 2 & 1;
    }

    bool getColorMaskA(u32 target) const
    {
        RIO_ASSERT(target < Graphics::RENDER_TARGET_MAX_NUM);
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 3 & 1;
    }

    bool getStencilTestEnable() const
    {
        return mStencilTestEnable;
    }

    Graphics::CompareFunc getStencilTestFunc() const
    {
        return mStencilTestFunc;
    }

    s32 getStencilTestRef() const
    {
        return mStencilTestRef;
    }

    u32 getStencilTestMask() const
    {
        return mStencilTestMask;
    }

    Graphics::StencilOp getStencilTestOpFail() const
    {
        return mStencilOpFail;
    }

    Graphics::StencilOp getStencilTestOpZFail() const
    {
        return mStencilOpZFail;
    }

    Graphics::StencilOp getStencilTestOpZPass() const
    {
        return mStencilOpZPass;
    }

    Graphics::PolygonMode getPolygonMode() const
    {
        return mPolygonMode;
    }

    bool getPolygonOffsetEnable() const
    {
        return mPolygonOffsetEnable;
    }

    bool getPolygonOffsetPointLineEnable() const
    {
        return mPolygonOffsetPointLineEnable;
    }

private:
    bool                    mDepthTestEnable;
    bool                    mDepthWriteEnable;
    Graphics::CompareFunc   mDepthFunc;
    Graphics::CullingMode   mCullingMode;
    BitFlag32               mBlendEnableMask;
    BlendExpression         mBlendExpression[Graphics::RENDER_TARGET_MAX_NUM];
    Color4f                 mBlendConstantColor;
    u32                     mColorMask;
    bool                    mStencilTestEnable;
    Graphics::CompareFunc   mStencilTestFunc;
    s32                     mStencilTestRef;
    u32                     mStencilTestMask;
    Graphics::StencilOp     mStencilOpFail;
    Graphics::StencilOp     mStencilOpZFail;
    Graphics::StencilOp     mStencilOpZPass;
    Graphics::PolygonMode   mPolygonMode;
    bool                    mPolygonOffsetEnable;
    bool                    mPolygonOffsetPointLineEnable;
};

}

#endif // RIO_GPU_RENDER_STATE_MRT_H
