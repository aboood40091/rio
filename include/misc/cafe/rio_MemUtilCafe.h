#ifndef RIO_MEM_UTIL_CAFE_H
#define RIO_MEM_UTIL_CAFE_H

// This file is included by rio_MemUtil.h
//#include <misc/rio_MemUtil.h>

#include <coreinit/memdefaultheap.h>
#include <coreinit/memory.h>

namespace rio {

inline void* MemUtil::copy(void* dst, const void* src, size_t size)
{
    RIO_ASSERT(src && dst && size);
    RIO_ASSERT((src < dst || src >= (u8*)dst + size) &&
               (dst < src || dst >= (u8*)src + size));

    return OSBlockMove(dst, src, size, 0);
}

inline void* MemUtil::set(void* ptr, u8 val, size_t size)
{
    RIO_ASSERT(ptr && size);

    return OSBlockSet(ptr, val, size);
}

inline void* MemUtil::alloc(size_t size, u32 alignment)
{
    RIO_ASSERT(size && alignment);

    return MEMAllocFromDefaultHeapEx(size, alignment);
}

inline void MemUtil::free(void* ptr)
{
    RIO_ASSERT(ptr);

    MEMFreeToDefaultHeap(ptr);
}

}

#endif // RIO_MEM_UTIL_CAFE_H
