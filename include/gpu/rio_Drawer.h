#ifndef RIO_GPU_DRAWER_H
#define RIO_GPU_DRAWER_H

#include <misc/rio_Types.h>

namespace rio {

class Drawer
{
public:
    enum PrimitiveMode : u32;   // Values are provided in each platform's Drawer header.
                                // Common ones are guaranteed to have the same enum name.

public:
    static constexpr u32    cVtxAlignment           = 0x40;
    static constexpr u32    cIdxAlignment           = 0x20;
    static constexpr u32    cUniformBlockAlignment  = 0x100;

public:
    static void DrawArraysInstanced(PrimitiveMode mode, u32 count, u32 instanceCount, u32 first = 0);
    static void DrawElementsInstanced(PrimitiveMode mode, u32 count, const u32* indices, u32 instanceCount);
    static void DrawElementsInstanced(PrimitiveMode mode, u32 count, const u16* indices, u32 instanceCount);
    static void DrawArrays(PrimitiveMode mode, u32 count, u32 first = 0);
    static void DrawElements(PrimitiveMode mode, u32 count, const u32* indices);
    static void DrawElements(PrimitiveMode mode, u32 count, const u16* indices);
};

}

#if RIO_IS_WIN
#include <gpu/win/rio_DrawerWin.h>
#elif RIO_IS_CAFE
#include <gpu/cafe/rio_DrawerCafe.h>
#endif

#endif // RIO_GPU_DRAWER_H
