#ifndef RIO_MEM_UTIL_WIN_H
#define RIO_MEM_UTIL_WIN_H

// This file is included by rio_MemUtil.h
//#include <misc/rio_MemUtil.h>

#include <cstring>

namespace rio {

inline void* MemUtil::copy(void* dst, const void* src, size_t size)
{
    RIO_ASSERT(src && dst && size);
    RIO_ASSERT((src < dst || src >= (u8*)dst + size) &&
               (dst < src || dst >= (u8*)src + size));

    return std::memcpy(dst, src, size);
}

inline void* MemUtil::set(void* ptr, u8 val, size_t size)
{
    RIO_ASSERT(ptr && size);

    return std::memset(ptr, val, size);
}

inline void* MemUtil::alloc(size_t size, u32 alignment)
{
    RIO_ASSERT(size && alignment);

    return new u8[size];
}

inline void MemUtil::free(void* ptr)
{
    RIO_ASSERT(ptr);

    delete[] (u8*)ptr;
}

}

#endif // RIO_MEM_UTIL_WIN_H
