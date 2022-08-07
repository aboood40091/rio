#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_VertexBuffer.h>

#include <gx2/mem.h>

namespace rio {

VertexBuffer::VertexBuffer(u32 buffer)
    : mpData(nullptr)
    , mSize(0)
    , mStride(0)
{
    RIO_ASSERT(buffer < NUM_MAX_BUFFERS);
    mBuffer = buffer;
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::setData(const void* data, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(mStride != 0);

    mpData = data;
    mSize = size;
}

void VertexBuffer::setDataInvalidate(const void* data, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(mStride != 0);

    mpData = data;
    mSize = size;

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, (void*)data, size);
}

void VertexBuffer::invalidateCache(const void* data, u32 size)
{
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER, (void*)data, size);
}

}

#endif // RIO_IS_CAFE
