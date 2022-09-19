#ifndef RIO_GPU_DRAWER_CAFE_H
#define RIO_GPU_DRAWER_CAFE_H

// This file is included by rio_Drawer.h
//#include <gpu/rio_Drawer.h>

#include <gx2/draw.h>

namespace rio {

enum Drawer::PrimitiveMode : u32
{
    POINTS                      = GX2_PRIMITIVE_MODE_POINTS,
    LINES                       = GX2_PRIMITIVE_MODE_LINES,
    LINE_STRIP                  = GX2_PRIMITIVE_MODE_LINE_STRIP,
    TRIANGLES                   = GX2_PRIMITIVE_MODE_TRIANGLES,
    TRIANGLE_FAN                = GX2_PRIMITIVE_MODE_TRIANGLE_FAN,
    TRIANGLE_STRIP              = GX2_PRIMITIVE_MODE_TRIANGLE_STRIP,
    LINES_ADJACENCY             = GX2_PRIMITIVE_MODE_LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY        = GX2_PRIMITIVE_MODE_LINE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY         = GX2_PRIMITIVE_MODE_TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY    = GX2_PRIMITIVE_MODE_TRIANGLE_STRIP_ADJACENCY,
    LINE_LOOP                   = GX2_PRIMITIVE_MODE_LINE_LOOP
};

inline void Drawer::DrawArraysInstanced(PrimitiveMode mode, u32 count, u32 instanceCount, u32 first)
{
    GX2DrawEx(static_cast<GX2PrimitiveMode>(mode), count, first, instanceCount);
}

inline void Drawer::DrawElementsInstanced(PrimitiveMode mode, u32 count, const u32* indices, u32 instanceCount)
{
    GX2DrawIndexedEx(static_cast<GX2PrimitiveMode>(mode), count, GX2_INDEX_TYPE_U32, indices, 0, instanceCount);
}

inline void Drawer::DrawElementsInstanced(PrimitiveMode mode, u32 count, const u16* indices, u32 instanceCount)
{
    GX2DrawIndexedEx(static_cast<GX2PrimitiveMode>(mode), count, GX2_INDEX_TYPE_U16, indices, 0, instanceCount);
}

inline void Drawer::DrawArrays(PrimitiveMode mode, u32 count, u32 first)
{
    DrawArraysInstanced(mode, count, 1, first);
}

inline void Drawer::DrawElements(PrimitiveMode mode, u32 count, const u32* indices)
{
    DrawElementsInstanced(mode, count, indices, 1);
}

inline void Drawer::DrawElements(PrimitiveMode mode, u32 count, const u16* indices)
{
    DrawElementsInstanced(mode, count, indices, 1);
}

}

#endif // RIO_GPU_DRAWER_CAFE_H
