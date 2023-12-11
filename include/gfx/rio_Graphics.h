#ifndef RIO_GFX_GRAPHICS_H
#define RIO_GFX_GRAPHICS_H

#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <misc/gl/rio_GL.h>
#define ENUM_VAL(key, val_win, val_cafe) key = val_win

#elif RIO_IS_CAFE

#include <gx2/enum.h>
#define ENUM_VAL(key, val_win, val_cafe) key = val_cafe

#else

#define ENUM_VAL(key, val_win, val_cafe) key

#endif

namespace rio {

class Window;

class Graphics
{
public:
    static void setViewport(s32 x, s32 y, u32 width, u32 height, f32 near = 0.0f, f32 far = 1.0f, s32 frame_buffer_height = -1);
    static void setScissor(s32 x, s32 y, u32 width, u32 height, s32 frame_buffer_height = -1);

public:
    enum CompareFunc
    {
        ENUM_VAL(COMPARE_FUNC_NEVER,    GL_NEVER,       GX2_COMPARE_FUNC_NEVER),
        ENUM_VAL(COMPARE_FUNC_LESS,     GL_LESS,        GX2_COMPARE_FUNC_LESS),
        ENUM_VAL(COMPARE_FUNC_EQUAL,    GL_EQUAL,       GX2_COMPARE_FUNC_EQUAL),
        ENUM_VAL(COMPARE_FUNC_LEQUAL,   GL_LEQUAL,      GX2_COMPARE_FUNC_LEQUAL),
        ENUM_VAL(COMPARE_FUNC_GREATER,  GL_GREATER,     GX2_COMPARE_FUNC_GREATER),
        ENUM_VAL(COMPARE_FUNC_NOTEQUAL, GL_NOTEQUAL,    GX2_COMPARE_FUNC_NOT_EQUAL),
        ENUM_VAL(COMPARE_FUNC_GEQUAL,   GL_GEQUAL,      GX2_COMPARE_FUNC_GEQUAL),
        ENUM_VAL(COMPARE_FUNC_ALWAYS,   GL_ALWAYS,      GX2_COMPARE_FUNC_ALWAYS)
    };

    enum CullingMode
    {
        CULLING_MODE_FRONT  = 0,
        CULLING_MODE_BACK   = 1,
        CULLING_MODE_NONE   = 2,
        CULLING_MODE_ALL    = 3
    };

    enum BlendFactor
    {
        ENUM_VAL(BLEND_MODE_ZERO,                       GL_ZERO,                        GX2_BLEND_MODE_ZERO),
        ENUM_VAL(BLEND_MODE_ONE,                        GL_ONE,                         GX2_BLEND_MODE_ONE),
        ENUM_VAL(BLEND_MODE_SRC_COLOR,                  GL_SRC_COLOR,                   GX2_BLEND_MODE_SRC_COLOR),
        ENUM_VAL(BLEND_MODE_ONE_MINUS_SRC_COLOR,        GL_ONE_MINUS_SRC_COLOR,         GX2_BLEND_MODE_INV_SRC_COLOR),
        ENUM_VAL(BLEND_MODE_SRC_ALPHA,                  GL_SRC_ALPHA,                   GX2_BLEND_MODE_SRC_ALPHA),
        ENUM_VAL(BLEND_MODE_ONE_MINUS_SRC_ALPHA,        GL_ONE_MINUS_SRC_ALPHA,         GX2_BLEND_MODE_INV_SRC_ALPHA),
        ENUM_VAL(BLEND_MODE_DST_ALPHA,                  GL_DST_ALPHA,                   GX2_BLEND_MODE_DST_ALPHA),
        ENUM_VAL(BLEND_MODE_ONE_MINUS_DST_ALPHA,        GL_ONE_MINUS_DST_ALPHA,         GX2_BLEND_MODE_INV_DST_ALPHA),
        ENUM_VAL(BLEND_MODE_DST_COLOR,                  GL_DST_COLOR,                   GX2_BLEND_MODE_DST_COLOR),
        ENUM_VAL(BLEND_MODE_ONE_MINUS_DST_COLOR,        GL_ONE_MINUS_DST_COLOR,         GX2_BLEND_MODE_INV_DST_COLOR),
        ENUM_VAL(BLEND_MODE_SRC_ALPHA_SATURATE,         GL_SRC_ALPHA_SATURATE,          GX2_BLEND_MODE_SRC_ALPHA_SAT),
        ENUM_VAL(BLEND_MODE_CONSTANT_COLOR,             GL_CONSTANT_COLOR,              GX2_BLEND_MODE_SRC1_COLOR),
        ENUM_VAL(BLEND_MODE_ONE_MINUS_CONSTANT_COLOR,   GL_ONE_MINUS_CONSTANT_COLOR,    GX2_BLEND_MODE_INV_SRC1_COLOR),
        ENUM_VAL(BLEND_MODE_CONSTANT_ALPHA,             GL_CONSTANT_ALPHA,              GX2_BLEND_MODE_SRC1_ALPHA),
        ENUM_VAL(BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA,   GL_ONE_MINUS_CONSTANT_ALPHA,    GX2_BLEND_MODE_INV_SRC1_ALPHA)
    };

    enum BlendEquation
    {
        ENUM_VAL(BLEND_FUNC_ADD,            GL_FUNC_ADD,                GX2_BLEND_COMBINE_MODE_ADD),
        ENUM_VAL(BLEND_FUNC_SUB,            GL_FUNC_SUBTRACT,           GX2_BLEND_COMBINE_MODE_SUB),
        ENUM_VAL(BLEND_FUNC_MIN,            GL_MIN,                     GX2_BLEND_COMBINE_MODE_MIN),
        ENUM_VAL(BLEND_FUNC_MAX,            GL_MAX,                     GX2_BLEND_COMBINE_MODE_MAX),
        ENUM_VAL(BLEND_FUNC_REVERSE_SUB,    GL_FUNC_REVERSE_SUBTRACT,   GX2_BLEND_COMBINE_MODE_REV_SUB)
    };

    enum StencilOp
    {
        ENUM_VAL(STENCIL_KEEP,      GL_KEEP,        GX2_STENCIL_FUNCTION_KEEP),
        ENUM_VAL(STENCIL_ZERO,      GL_ZERO,        GX2_STENCIL_FUNCTION_ZERO),
        ENUM_VAL(STENCIL_REPLACE,   GL_REPLACE,     GX2_STENCIL_FUNCTION_REPLACE),
        ENUM_VAL(STENCIL_INCR,      GL_INCR,        GX2_STENCIL_FUNCTION_INCR_CLAMP),
        ENUM_VAL(STENCIL_DECR,      GL_DECR,        GX2_STENCIL_FUNCTION_DECR_CLAMP),
        ENUM_VAL(STENCIL_INVERT,    GL_INVERT,      GX2_STENCIL_FUNCTION_INV),
        ENUM_VAL(STENCIL_INCR_WRAP, GL_INCR_WRAP,   GX2_STENCIL_FUNCTION_INCR_WRAP),
        ENUM_VAL(STENCIL_DECR_WRAP, GL_DECR_WRAP,   GX2_STENCIL_FUNCTION_DECR_WRAP)
    };

    enum PolygonMode
    {
        ENUM_VAL(POLYGON_MODE_POINT,    GL_POINT,   GX2_POLYGON_MODE_POINT),
        ENUM_VAL(POLYGON_MODE_LINE,     GL_LINE,    GX2_POLYGON_MODE_LINE),
        ENUM_VAL(POLYGON_MODE_FILL,     GL_FILL,    GX2_POLYGON_MODE_TRIANGLE)
    };

    enum
    {
        RENDER_TARGET_MAX_NUM   = 8
    };

private:
    static s32  sViewportX,     sViewportY;
    static u32  sViewportWidth, sViewportHeight;
    static f32  sViewportNear,  sViewportFar;
    static s32  sScissorX,      sScissorY;
    static u32  sScissorWidth,  sScissorHeight;

    friend class Window;
};

}

#endif // RIO_GFX_GRAPHICS_H
