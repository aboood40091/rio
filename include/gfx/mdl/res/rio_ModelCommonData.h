#ifndef RIO_GFX_MDL_RES_MODEL_COMMON_DATA_H
#define RIO_GFX_MDL_RES_MODEL_COMMON_DATA_H

#include <math/rio_MathTypes.h>

#include <type_traits>

namespace rio { namespace mdl { namespace res {

template <typename T>
class OffsetPtr
{
    // Class representing a relative offset field to an object

public:
    T* ptr()
    {
        if (mRelaOffset)
            return (T*)((uintptr_t)&mRelaOffset + mRelaOffset);

        return nullptr;
    }

    const T* ptr() const
    {
        if (mRelaOffset)
            return (const T*)((uintptr_t)&mRelaOffset + mRelaOffset);

        return nullptr;
    }

    T& ref()
    {
        RIO_ASSERT(mRelaOffset);
        return *ptr();
    }

    const T& ref() const
    {
        RIO_ASSERT(mRelaOffset);
        return *ptr();
    }

    OffsetPtr() = default;
    OffsetPtr(const OffsetPtr&) = delete;
    OffsetPtr& operator=(const OffsetPtr&) = delete;

private:
    s32 mRelaOffset; // Offset to object
};
static_assert(std::is_pod<OffsetPtr<u8>>::value);
static_assert(sizeof(OffsetPtr<u8>) == 4);

template <typename T>
class Buffer
{
    // Class representing a relative offset field to a buffer

public:
    T* ptr()
    {
        return mOffset.ptr();
    }

    const T* ptr() const
    {
        return mOffset.ptr();
    }

    u32 count() const
    {
        return mBufferCount;
    }

    u32 size() const
    {
        return mBufferCount * sizeof(T);
    }

private:
    OffsetPtr<T>    mOffset;      // Buffer offset.
    u32             mBufferCount; // Buffer element count.
};
static_assert(std::is_pod<Buffer<u8>>::value);
static_assert(sizeof(Buffer<u8>) == 8);

typedef Buffer<u8>          BufferU8;
typedef Buffer<u16>         BufferU16;
typedef Buffer<u32>         BufferU32;
typedef Buffer<f32>         BufferF32;

typedef Buffer<s8>          BufferS8;
typedef Buffer<s16>         BufferS16;
typedef Buffer<s32>         BufferS32;
typedef Buffer<f64>         BufferF64;

typedef Buffer<BaseVec2i>   BufferVec2i;
typedef Buffer<BaseVec3i>   BufferVec3i;
typedef Buffer<BaseVec4i>   BufferVec4i;

typedef Buffer<BaseVec2f>   BufferVec2f;
typedef Buffer<BaseVec3f>   BufferVec3f;
typedef Buffer<BaseVec4f>   BufferVec4f;

typedef Buffer<char>        FixedString;

} } }

#endif // RIO_GFX_MDL_RES_MODEL_COMMON_DATA_H
