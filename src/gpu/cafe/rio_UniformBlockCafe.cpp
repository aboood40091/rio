#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_Drawer.h>
#include <gpu/rio_UniformBlock.h>

#include <gx2/mem.h>
#include <gx2/shaders.h>

namespace rio {

UniformBlock::UniformBlock(ShaderStage stage, u32 index)
    : mVSIndex(index)
    , mFSIndex(index)
    , mpData(nullptr)
    , mSize(0)
    , mStage(stage)
{
}

UniformBlock::UniformBlock(ShaderStage stage, u32 vs_index, u32 fs_index)
    : mVSIndex(vs_index)
    , mFSIndex(fs_index)
    , mpData(nullptr)
    , mSize(0)
    , mStage(stage)
{
}

UniformBlock::~UniformBlock()
{
}

void UniformBlock::setData(const void* data, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);

    // Additional check for Cafe
    RIO_ASSERT(uintptr_t(data) % Drawer::cUniformBlockAlignment == 0);
    RIO_ASSERT(size % sizeof(u32) == 0);

    mpData = data;
    mSize = size;

    const u32* src = (const u32*)data;
    u32* dst = (u32*)mpData;
    const u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
        dst[i] = __builtin_bswap32(src[i]);
}

void UniformBlock::setSubData(const void* data, u32 offset, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(mpData != nullptr);
    RIO_ASSERT(offset + size <= mSize);

    // Additional checks for Cafe
    RIO_ASSERT(offset % sizeof(u32) == 0);
    RIO_ASSERT(size   % sizeof(u32) == 0);

    const u32* src = (const u32*)data;
    u32* dst = (u32*)mpData + (offset / sizeof(u32));
    const u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
        dst[i] = __builtin_bswap32(src[i]);
}

void UniformBlock::setDataInvalidate(const void* data, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);

    // Additional check for Cafe
    RIO_ASSERT(uintptr_t(data) % Drawer::cUniformBlockAlignment == 0);
    RIO_ASSERT(size % sizeof(u32) == 0);

    mpData = data;
    mSize = size;

    const u32* src = (const u32*)data;
    u32* dst = (u32*)mpData;
    const u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
        dst[i] = __builtin_bswap32(src[i]);

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU |
                  GX2_INVALIDATE_MODE_UNIFORM_BLOCK, (void*)dst, size);
}

void UniformBlock::setSubDataInvalidate(const void* data, u32 offset, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(mpData != nullptr);
    RIO_ASSERT(offset + size <= mSize);

    // Additional checks for Cafe
    RIO_ASSERT(offset % sizeof(u32) == 0);
    RIO_ASSERT(size   % sizeof(u32) == 0);

    const u32* src = (const u32*)data;
    u32* dst = (u32*)mpData + (offset / sizeof(u32));
    const u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
        dst[i] = __builtin_bswap32(src[i]);

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU |
                  GX2_INVALIDATE_MODE_UNIFORM_BLOCK, (void*)dst, size);
}

void UniformBlock::invalidateCache(const void* data, u32 size)
{
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU |
                  GX2_INVALIDATE_MODE_UNIFORM_BLOCK, (void*)data, size);
}

void UniformBlock::bind() const
{
    if (mStage & STAGE_VERTEX_SHADER)
    {
        RIO_ASSERT(mVSIndex != u32(-1));
        GX2SetVertexUniformBlock(mVSIndex, mSize, mpData);
    }

    if (mStage & STAGE_FRAGMENT_SHADER)
    {
        RIO_ASSERT(mFSIndex != u32(-1));
        GX2SetPixelUniformBlock(mFSIndex, mSize, mpData);
    }
}

}

#endif // RIO_IS_CAFE
