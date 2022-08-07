#ifndef RIO_MEM_UTIL_H
#define RIO_MEM_UTIL_H

#include <misc/rio_Types.h>

namespace rio {

class MemUtil
{
public:
    static void* copy(void* dst, const void* src, size_t size);
    static void* set(void* ptr, u8 val, size_t size);

    static void* alloc(size_t size, u32 alignment);
    static void free(void* ptr);
};

}

#if RIO_IS_WIN
#include <misc/win/rio_MemUtilWin.h>
#elif RIO_IS_CAFE
#include <misc/cafe/rio_MemUtilCafe.h>
#endif

#endif // RIO_MEM_UTIL_H
