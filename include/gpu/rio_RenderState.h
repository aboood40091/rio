#ifndef RIO_GPU_RENDER_STATE_H
#define RIO_GPU_RENDER_STATE_H

#include <gfx/rio_Color.h>
#include <gfx/rio_Graphics.h>

namespace rio {

class RenderState
{
public:
    RenderState();

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

    void setBlendEnable(bool blend)
    {
        mBlendEnable = blend;
    }

    void setBlendFactor(Graphics::BlendFactor src_factor, Graphics::BlendFactor dst_factor)
    {
        setBlendFactorSrc(src_factor);
        setBlendFactorDst(dst_factor);
    }

    void setBlendFactorSeparate(Graphics::BlendFactor src_factor_rgb, Graphics::BlendFactor dst_factor_rgb, Graphics::BlendFactor src_factor_a, Graphics::BlendFactor dst_factor_a)
    {
        setBlendFactorSrcRGB(src_factor_rgb);
        setBlendFactorDstRGB(dst_factor_rgb);
        setBlendFactorSrcAlpha(src_factor_a);
        setBlendFactorDstAlpha(dst_factor_a);
    }

    void setBlendFactorSrc(Graphics::BlendFactor factor)
    {
        setBlendFactorSrcRGB(factor);
        setBlendFactorSrcAlpha(factor);
    }

    void setBlendFactorDst(Graphics::BlendFactor factor)
    {
        setBlendFactorDstRGB(factor);
        setBlendFactorDstAlpha(factor);
    }

    void setBlendFactorSrcRGB(Graphics::BlendFactor factor)
    {
        mBlendFactorSrcRGB = factor;
    }

    void setBlendFactorSrcAlpha(Graphics::BlendFactor factor)
    {
        mBlendFactorSrcA = factor;
    }

    void setBlendFactorDstRGB(Graphics::BlendFactor factor)
    {
        mBlendFactorDstRGB = factor;
    }

    void setBlendFactorDstAlpha(Graphics::BlendFactor factor)
    {
        mBlendFactorDstA = factor;
    }

    void setBlendEquation(Graphics::BlendEquation equation)
    {
        setBlendEquationRGB(equation);
        setBlendEquationAlpha(equation);
    }

    void setBlendEquationSeparate(Graphics::BlendEquation equation_rgb, Graphics::BlendEquation equation_a)
    {
        setBlendEquationRGB(equation_rgb);
        setBlendEquationAlpha(equation_a);
    }

    void setBlendEquationRGB(Graphics::BlendEquation equation)
    {
        mBlendEquationRGB = equation;
    }

    void setBlendEquationAlpha(Graphics::BlendEquation equation)
    {
        mBlendEquationA = equation;
    }

    void setBlendConstantColor(const Color4f& color)
    {
        mBlendConstantColor = color;
    }

    void setColorMask(bool r, bool g, bool b, bool a)
    {
        mColorMaskR = r;
        mColorMaskG = g;
        mColorMaskB = b;
        mColorMaskA = a;
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

    bool getBlendEnable() const
    {
        return mBlendEnable;
    }

    Graphics::BlendFactor getBlendFactorSrcRGB() const
    {
        return mBlendFactorSrcRGB;
    }

    Graphics::BlendFactor getBlendFactorSrcAlpha() const
    {
        return mBlendFactorSrcA;
    }

    Graphics::BlendFactor getBlendFactorDstRGB() const
    {
        return mBlendFactorDstRGB;
    }

    Graphics::BlendFactor getBlendFactorDstAlpha() const
    {
        return mBlendFactorDstA;
    }

    Graphics::BlendEquation getBlendEquationRGB() const
    {
        return mBlendEquationRGB;
    }

    Graphics::BlendEquation getBlendEquationAlpha() const
    {
        return mBlendEquationA;
    }

    const Color4f& getBlendConstantColor() const
    {
        return mBlendConstantColor;
    }

    bool getColorMaskR() const
    {
        return mColorMaskR;
    }

    bool getColorMaskG() const
    {
        return mColorMaskG;
    }

    bool getColorMaskB() const
    {
        return mColorMaskB;
    }

    bool getColorMaskA() const
    {
        return mColorMaskA;
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
    bool                    mBlendEnable;
    Graphics::BlendFactor   mBlendFactorSrcRGB;
    Graphics::BlendFactor   mBlendFactorSrcA;
    Graphics::BlendFactor   mBlendFactorDstRGB;
    Graphics::BlendFactor   mBlendFactorDstA;
    Graphics::BlendEquation mBlendEquationRGB;
    Graphics::BlendEquation mBlendEquationA;
    Color4f                 mBlendConstantColor;
    bool                    mColorMaskR;
    bool                    mColorMaskG;
    bool                    mColorMaskB;
    bool                    mColorMaskA;
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

#endif // RIO_GPU_RENDER_STATE_H
