#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_VertexBuffer.h>

#include <GL/glew.h>

namespace rio {

VertexBuffer::VertexBuffer(u32 buffer)
    : mpData(nullptr)
    , mSize(0)
    , mStride(0)
{
    RIO_ASSERT(buffer < NUM_MAX_BUFFERS);
    mBuffer = buffer;

    glGenBuffers(1, &mHandle);
    RIO_ASSERT(mHandle != GL_NONE);
}

VertexBuffer::~VertexBuffer()
{
    if (mHandle != GL_NONE)
    {
        glDeleteBuffers(1, &mHandle);
        mHandle = GL_NONE;
    }
}

void VertexBuffer::setData(const void* data, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(mStride != 0);

    glBindBuffer(GL_ARRAY_BUFFER, mHandle);

    if (size == mSize)
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    else
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

    mpData = data;
    mSize = size;
}

void VertexBuffer::setSubData(const void* data, u32 offset, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(mStride != 0);
    RIO_ASSERT(offset + size <= mSize);

    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

}

#endif // RIO_IS_WIN
