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

    mpData = data;
    mSize = size;

    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

}

#endif // RIO_IS_WIN
