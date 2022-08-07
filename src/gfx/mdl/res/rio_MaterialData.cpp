#include <gfx/mdl/res/rio_MaterialData.h>
#include <gpu/rio_RenderState.h>

namespace rio { namespace mdl { namespace res {

void Material::initRenderState(RenderState& state) const
{
    state.setDepthTestEnable(mRenderFlags & DEPTH_TEST_ENABLE);
    state.setDepthWriteEnable(mRenderFlags & DEPTH_WRITE_ENABLE);
    {
        Graphics::CompareFunc func;
        bool found = true;

        switch (mDepthFunc)
        {
        case COMPARE_FUNC_NEVER:    func = Graphics::COMPARE_FUNC_NEVER;    break;
        case COMPARE_FUNC_LESS:     func = Graphics::COMPARE_FUNC_LESS;     break;
        case COMPARE_FUNC_EQUAL:    func = Graphics::COMPARE_FUNC_EQUAL;    break;
        case COMPARE_FUNC_LEQUAL:   func = Graphics::COMPARE_FUNC_LEQUAL;   break;
        case COMPARE_FUNC_GREATER:  func = Graphics::COMPARE_FUNC_GREATER;  break;
        case COMPARE_FUNC_NOTEQUAL: func = Graphics::COMPARE_FUNC_NOTEQUAL; break;
        case COMPARE_FUNC_GEQUAL:   func = Graphics::COMPARE_FUNC_GEQUAL;   break;
        case COMPARE_FUNC_ALWAYS:   func = Graphics::COMPARE_FUNC_ALWAYS;   break;
        default: found = false;
        }

        if (found)
            state.setDepthFunc(func);
    }

    {
        Graphics::CullingMode mode;
        bool found = true;

        switch (mCullingMode)
        {
        case CULLING_MODE_FRONT: mode = Graphics::CULLING_MODE_FRONT; break;
        case CULLING_MODE_BACK:  mode = Graphics::CULLING_MODE_BACK;  break;
        case CULLING_MODE_NONE:  mode = Graphics::CULLING_MODE_NONE;  break;
        case CULLING_MODE_ALL:   mode = Graphics::CULLING_MODE_ALL;   break;
        default: found = false;
        }

        if (found)
            state.setCullingMode(mode);
    }

    state.setBlendEnable(mRenderFlags & BLEND_ENABLE);

    {
        Graphics::BlendFactor src_factor_rgb;
        bool found = true;

        switch (mBlendFactorSrcRGB)
        {
        case BLEND_MODE_ZERO:                     src_factor_rgb = Graphics::BLEND_MODE_ZERO;                     break;
        case BLEND_MODE_ONE:                      src_factor_rgb = Graphics::BLEND_MODE_ONE;                      break;
        case BLEND_MODE_SRC_COLOR:                src_factor_rgb = Graphics::BLEND_MODE_SRC_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_SRC_COLOR:      src_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_SRC_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA:                src_factor_rgb = Graphics::BLEND_MODE_SRC_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_SRC_ALPHA:      src_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA;      break;
        case BLEND_MODE_DST_ALPHA:                src_factor_rgb = Graphics::BLEND_MODE_DST_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_DST_ALPHA:      src_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_DST_ALPHA;      break;
        case BLEND_MODE_DST_COLOR:                src_factor_rgb = Graphics::BLEND_MODE_DST_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_DST_COLOR:      src_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_DST_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA_SATURATE:       src_factor_rgb = Graphics::BLEND_MODE_SRC_ALPHA_SATURATE;       break;
        case BLEND_MODE_CONSTANT_COLOR:           src_factor_rgb = Graphics::BLEND_MODE_CONSTANT_COLOR;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_COLOR: src_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_COLOR; break;
        case BLEND_MODE_CONSTANT_ALPHA:           src_factor_rgb = Graphics::BLEND_MODE_CONSTANT_ALPHA;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA: src_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA; break;
        default: found = false;
        }

        if (found)
            state.setBlendFactorSrcRGB(src_factor_rgb);
    }

    {
        Graphics::BlendFactor src_factor_a;
        bool found = true;

        switch (mBlendFactorSrcA)
        {
        case BLEND_MODE_ZERO:                     src_factor_a = Graphics::BLEND_MODE_ZERO;                     break;
        case BLEND_MODE_ONE:                      src_factor_a = Graphics::BLEND_MODE_ONE;                      break;
        case BLEND_MODE_SRC_COLOR:                src_factor_a = Graphics::BLEND_MODE_SRC_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_SRC_COLOR:      src_factor_a = Graphics::BLEND_MODE_ONE_MINUS_SRC_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA:                src_factor_a = Graphics::BLEND_MODE_SRC_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_SRC_ALPHA:      src_factor_a = Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA;      break;
        case BLEND_MODE_DST_ALPHA:                src_factor_a = Graphics::BLEND_MODE_DST_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_DST_ALPHA:      src_factor_a = Graphics::BLEND_MODE_ONE_MINUS_DST_ALPHA;      break;
        case BLEND_MODE_DST_COLOR:                src_factor_a = Graphics::BLEND_MODE_DST_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_DST_COLOR:      src_factor_a = Graphics::BLEND_MODE_ONE_MINUS_DST_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA_SATURATE:       src_factor_a = Graphics::BLEND_MODE_SRC_ALPHA_SATURATE;       break;
        case BLEND_MODE_CONSTANT_COLOR:           src_factor_a = Graphics::BLEND_MODE_CONSTANT_COLOR;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_COLOR: src_factor_a = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_COLOR; break;
        case BLEND_MODE_CONSTANT_ALPHA:           src_factor_a = Graphics::BLEND_MODE_CONSTANT_ALPHA;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA: src_factor_a = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA; break;
        default: found = false;
        }

        if (found)
            state.setBlendFactorSrcAlpha(src_factor_a);
    }

    {
        Graphics::BlendFactor dst_factor_rgb;
        bool found = true;

        switch (mBlendFactorDstRGB)
        {
        case BLEND_MODE_ZERO:                     dst_factor_rgb = Graphics::BLEND_MODE_ZERO;                     break;
        case BLEND_MODE_ONE:                      dst_factor_rgb = Graphics::BLEND_MODE_ONE;                      break;
        case BLEND_MODE_SRC_COLOR:                dst_factor_rgb = Graphics::BLEND_MODE_SRC_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_SRC_COLOR:      dst_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_SRC_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA:                dst_factor_rgb = Graphics::BLEND_MODE_SRC_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_SRC_ALPHA:      dst_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA;      break;
        case BLEND_MODE_DST_ALPHA:                dst_factor_rgb = Graphics::BLEND_MODE_DST_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_DST_ALPHA:      dst_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_DST_ALPHA;      break;
        case BLEND_MODE_DST_COLOR:                dst_factor_rgb = Graphics::BLEND_MODE_DST_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_DST_COLOR:      dst_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_DST_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA_SATURATE:       dst_factor_rgb = Graphics::BLEND_MODE_SRC_ALPHA_SATURATE;       break;
        case BLEND_MODE_CONSTANT_COLOR:           dst_factor_rgb = Graphics::BLEND_MODE_CONSTANT_COLOR;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_COLOR: dst_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_COLOR; break;
        case BLEND_MODE_CONSTANT_ALPHA:           dst_factor_rgb = Graphics::BLEND_MODE_CONSTANT_ALPHA;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA: dst_factor_rgb = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA; break;
        default: found = false;
        }

        if (found)
            state.setBlendFactorDstRGB(dst_factor_rgb);
    }

    {
        Graphics::BlendFactor dst_factor_a;
        bool found = true;

        switch (mBlendFactorDstA)
        {
        case BLEND_MODE_ZERO:                     dst_factor_a = Graphics::BLEND_MODE_ZERO;                     break;
        case BLEND_MODE_ONE:                      dst_factor_a = Graphics::BLEND_MODE_ONE;                      break;
        case BLEND_MODE_SRC_COLOR:                dst_factor_a = Graphics::BLEND_MODE_SRC_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_SRC_COLOR:      dst_factor_a = Graphics::BLEND_MODE_ONE_MINUS_SRC_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA:                dst_factor_a = Graphics::BLEND_MODE_SRC_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_SRC_ALPHA:      dst_factor_a = Graphics::BLEND_MODE_ONE_MINUS_SRC_ALPHA;      break;
        case BLEND_MODE_DST_ALPHA:                dst_factor_a = Graphics::BLEND_MODE_DST_ALPHA;                break;
        case BLEND_MODE_ONE_MINUS_DST_ALPHA:      dst_factor_a = Graphics::BLEND_MODE_ONE_MINUS_DST_ALPHA;      break;
        case BLEND_MODE_DST_COLOR:                dst_factor_a = Graphics::BLEND_MODE_DST_COLOR;                break;
        case BLEND_MODE_ONE_MINUS_DST_COLOR:      dst_factor_a = Graphics::BLEND_MODE_ONE_MINUS_DST_COLOR;      break;
        case BLEND_MODE_SRC_ALPHA_SATURATE:       dst_factor_a = Graphics::BLEND_MODE_SRC_ALPHA_SATURATE;       break;
        case BLEND_MODE_CONSTANT_COLOR:           dst_factor_a = Graphics::BLEND_MODE_CONSTANT_COLOR;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_COLOR: dst_factor_a = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_COLOR; break;
        case BLEND_MODE_CONSTANT_ALPHA:           dst_factor_a = Graphics::BLEND_MODE_CONSTANT_ALPHA;           break;
        case BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA: dst_factor_a = Graphics::BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA; break;
        default: found = false;
        }

        if (found)
            state.setBlendFactorDstAlpha(dst_factor_a);
    }

    {
        Graphics::BlendEquation equation;
        bool found = true;

        switch (mBlendEquationRGB)
        {
        case BLEND_FUNC_ADD:         equation = Graphics::BLEND_FUNC_ADD;         break;
        case BLEND_FUNC_SUB:         equation = Graphics::BLEND_FUNC_SUB;         break;
        case BLEND_FUNC_MIN:         equation = Graphics::BLEND_FUNC_MIN;         break;
        case BLEND_FUNC_MAX:         equation = Graphics::BLEND_FUNC_MAX;         break;
        case BLEND_FUNC_REVERSE_SUB: equation = Graphics::BLEND_FUNC_REVERSE_SUB; break;
        default: found = false;
        }

        if (found)
            state.setBlendEquationRGB(equation);
    }

    {
        Graphics::BlendEquation equation;
        bool found = true;

        switch (mBlendEquationA)
        {
        case BLEND_FUNC_ADD:         equation = Graphics::BLEND_FUNC_ADD;         break;
        case BLEND_FUNC_SUB:         equation = Graphics::BLEND_FUNC_SUB;         break;
        case BLEND_FUNC_MIN:         equation = Graphics::BLEND_FUNC_MIN;         break;
        case BLEND_FUNC_MAX:         equation = Graphics::BLEND_FUNC_MAX;         break;
        case BLEND_FUNC_REVERSE_SUB: equation = Graphics::BLEND_FUNC_REVERSE_SUB; break;
        default: found = false;
        }

        if (found)
            state.setBlendEquationAlpha(equation);
    }

    state.setBlendConstantColor(mBlendConstantColor);

    // Alpha testing was removed because core profile of OpenGL is required now
    /*
    state.setAlphaTestEnable(mRenderFlags & ALPHA_TEST_ENABLE);
    {
        Graphics::CompareFunc func;
        bool found = true;

        switch (mAlphaTestFunc)
        {
        case COMPARE_FUNC_NEVER:    func = Graphics::COMPARE_FUNC_NEVER;    break;
        case COMPARE_FUNC_LESS:     func = Graphics::COMPARE_FUNC_LESS;     break;
        case COMPARE_FUNC_EQUAL:    func = Graphics::COMPARE_FUNC_EQUAL;    break;
        case COMPARE_FUNC_LEQUAL:   func = Graphics::COMPARE_FUNC_LEQUAL;   break;
        case COMPARE_FUNC_GREATER:  func = Graphics::COMPARE_FUNC_GREATER;  break;
        case COMPARE_FUNC_NOTEQUAL: func = Graphics::COMPARE_FUNC_NOTEQUAL; break;
        case COMPARE_FUNC_GEQUAL:   func = Graphics::COMPARE_FUNC_GEQUAL;   break;
        case COMPARE_FUNC_ALWAYS:   func = Graphics::COMPARE_FUNC_ALWAYS;   break;
        default: found = false;
        }

        if (found)
            state.setAlphaTestFunc(func, mAlphaTestRef);
    }
    */

    state.setColorMask(mRenderFlags & COLOR_MASK_R,
                       mRenderFlags & COLOR_MASK_G,
                       mRenderFlags & COLOR_MASK_B,
                       mRenderFlags & COLOR_MASK_A);

    state.setStencilTestEnable(mRenderFlags & STENCIL_TEST_ENABLE);
    {
        Graphics::CompareFunc func;
        bool found = true;

        switch (mStencilTestFunc)
        {
        case COMPARE_FUNC_NEVER:    func = Graphics::COMPARE_FUNC_NEVER;    break;
        case COMPARE_FUNC_LESS:     func = Graphics::COMPARE_FUNC_LESS;     break;
        case COMPARE_FUNC_EQUAL:    func = Graphics::COMPARE_FUNC_EQUAL;    break;
        case COMPARE_FUNC_LEQUAL:   func = Graphics::COMPARE_FUNC_LEQUAL;   break;
        case COMPARE_FUNC_GREATER:  func = Graphics::COMPARE_FUNC_GREATER;  break;
        case COMPARE_FUNC_NOTEQUAL: func = Graphics::COMPARE_FUNC_NOTEQUAL; break;
        case COMPARE_FUNC_GEQUAL:   func = Graphics::COMPARE_FUNC_GEQUAL;   break;
        case COMPARE_FUNC_ALWAYS:   func = Graphics::COMPARE_FUNC_ALWAYS;   break;
        default: found = false;
        }

        if (found)
            state.setStencilTestFunc(func, mStencilTestRef, mStencilTestMask);
    }

    {
        Graphics::StencilOp fail = Graphics::STENCIL_KEEP;
        Graphics::StencilOp zfail = Graphics::STENCIL_KEEP;
        Graphics::StencilOp zpass = Graphics::STENCIL_KEEP;

        bool found = true;

        switch (mStencilOpFail)
        {
        case STENCIL_KEEP:      fail = Graphics::STENCIL_KEEP;      break;
        case STENCIL_ZERO:      fail = Graphics::STENCIL_ZERO;      break;
        case STENCIL_REPLACE:   fail = Graphics::STENCIL_REPLACE;   break;
        case STENCIL_INCR:      fail = Graphics::STENCIL_INCR;      break;
        case STENCIL_DECR:      fail = Graphics::STENCIL_DECR;      break;
        case STENCIL_INVERT:    fail = Graphics::STENCIL_INVERT;    break;
        case STENCIL_INCR_WRAP: fail = Graphics::STENCIL_INCR_WRAP; break;
        case STENCIL_DECR_WRAP: fail = Graphics::STENCIL_DECR_WRAP; break;
        default: found = false;
        }

        switch (mStencilOpZFail)
        {
        case STENCIL_KEEP:      zfail = Graphics::STENCIL_KEEP;      break;
        case STENCIL_ZERO:      zfail = Graphics::STENCIL_ZERO;      break;
        case STENCIL_REPLACE:   zfail = Graphics::STENCIL_REPLACE;   break;
        case STENCIL_INCR:      zfail = Graphics::STENCIL_INCR;      break;
        case STENCIL_DECR:      zfail = Graphics::STENCIL_DECR;      break;
        case STENCIL_INVERT:    zfail = Graphics::STENCIL_INVERT;    break;
        case STENCIL_INCR_WRAP: zfail = Graphics::STENCIL_INCR_WRAP; break;
        case STENCIL_DECR_WRAP: zfail = Graphics::STENCIL_DECR_WRAP; break;
        default: found = false;
        }

        switch (mStencilOpZPass)
        {
        case STENCIL_KEEP:      zpass = Graphics::STENCIL_KEEP;      break;
        case STENCIL_ZERO:      zpass = Graphics::STENCIL_ZERO;      break;
        case STENCIL_REPLACE:   zpass = Graphics::STENCIL_REPLACE;   break;
        case STENCIL_INCR:      zpass = Graphics::STENCIL_INCR;      break;
        case STENCIL_DECR:      zpass = Graphics::STENCIL_DECR;      break;
        case STENCIL_INVERT:    zpass = Graphics::STENCIL_INVERT;    break;
        case STENCIL_INCR_WRAP: zpass = Graphics::STENCIL_INCR_WRAP; break;
        case STENCIL_DECR_WRAP: zpass = Graphics::STENCIL_DECR_WRAP; break;
        default: found = false;
        }

        if (found)
            state.setStencilTestOp(fail, zfail, zpass);
    }

    {
        Graphics::PolygonMode mode;
        bool found = true;

        switch (mPolygonMode)
        {
        case POLYGON_MODE_POINT: mode = Graphics::POLYGON_MODE_POINT; break;
        case POLYGON_MODE_LINE:  mode = Graphics::POLYGON_MODE_LINE;  break;
        case POLYGON_MODE_FILL:  mode = Graphics::POLYGON_MODE_FILL;  break;
        default: found = false;
        }

        if (found)
            state.setPolygonMode(mode);
    }

    state.setPolygonOffsetEnable(mRenderFlags & POLYGON_OFFSET_ENABLE,
                                 mRenderFlags & POLYGON_OFFSET_POINT_LINE_ENABLE);
}

} } }
