#ifndef RIO_GPU_VERTEX_BUFFER_H
#define RIO_GPU_VERTEX_BUFFER_H

#include <gpu/rio_VertexStream.h>

namespace rio {

class VertexArray;

class VertexBuffer
{
    // Wrapper class representing a buffer of vertex attribute data.

    // Note: On Cafe, data is passed directly to the GPU, therefore it must not be
    //       freed as long as this vertex buffer is being used.

public:
    enum
    {
        // Maximum number of concurrent vertex buffers per drawcall.
        NUM_MAX_BUFFERS = 16
    };

public:
    VertexBuffer(u32 buffer = 0);
    ~VertexBuffer();

private:
    VertexBuffer(const VertexBuffer&);
    VertexBuffer& operator=(const VertexBuffer&);

public:
    VertexBuffer(const void* data, u32 size, u32 stride, u32 buffer = 0)
        : VertexBuffer(buffer)
    {
        setStride(stride);
        setDataInvalidate(data, size);
    }

    const void* getData() const { return mpData; }
    u32 getSize() const { return mSize; }
    u32 getStride() const { return mStride; }

    void setStride(u32 stride)
    {
        RIO_ASSERT(stride != 0);
        mStride = stride;
    }

    // Sets the passed data pointer as this object's data buffer.
    void setData(const void* data, u32 size);
    // Copies the passed data into the specified range.
    void setSubData(const void* data, u32 offset, u32 size);

    // Setter functions with cache invalidation (same rules as above apply, currently only useful for Cafe):
    void setDataInvalidate(const void* data, u32 size);
    void setSubDataInvalidate(const void* data, u32 offset, u32 size);

    static void invalidateCache(const void* data, u32 size);

    // This object is bound by VertexArray

private:
    u32                 mBuffer;    // Buffer index (0-15)
#if RIO_IS_WIN
    u32                 mHandle;    // Buffer handle (for OpenGL)
#endif // RIO_IS_WIN
    const void*         mpData;     // Buffer data
    u32                 mSize;      // Buffer size
    u32                 mStride;    // Vertex Stride
    VertexStream::List  mStreams;   // List of streams contained in this buffer
                                    // (Managed by VertexArray)

    friend class VertexArray;
};

#if RIO_IS_WIN

inline void VertexBuffer::setDataInvalidate(const void* data, u32 size)
{
    setData(data, size);
}

inline void VertexBuffer::setSubDataInvalidate(const void* data, u32 offset, u32 size)
{
    setSubData(data, offset, size);
}

inline void VertexBuffer::invalidateCache(const void* data, u32 size)
{
}

#endif // RIO_IS_WIN

}

#endif // RIO_GPU_VERTEX_BUFFER_H
