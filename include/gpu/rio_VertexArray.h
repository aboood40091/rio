#ifndef RIO_GPU_VERTEX_ARRAY_H
#define RIO_GPU_VERTEX_ARRAY_H

#include <gpu/rio_VertexBuffer.h>

#include <cstring>

namespace rio {

class VertexArray
{
public:
    VertexArray()
#if RIO_IS_CAFE
        : mpFetchShaderBuf(nullptr)
        , mFetchShaderBufSize(0)
#elif RIO_IS_WIN
        : mHandle(0)
#endif
    {
        // Clear vertex buffers list
        std::memset(mpVertexBuffer, 0, sizeof(VertexBuffer*) * VertexBuffer::NUM_MAX_BUFFERS);
        initialize();
    }

    ~VertexArray();

private:
    VertexArray(const VertexArray&);
    VertexArray& operator=(const VertexArray&);

public:
    // Clear vertex array of all previous data
    void initialize();

    // Add vertex attribute stream with the specified vertex buffer
    void addAttribute(VertexStream& stream, VertexBuffer& vertex_buffer)
    {
        vertex_buffer.mStreams.pushBack(&stream);
        stream.mpVertexBuffer = &vertex_buffer;

        mpVertexBuffer[vertex_buffer.mBuffer] = &vertex_buffer;
    }

    // Process all added vertex attribute streams
    void process();

    // Bind vertex array
    // Note: On Cafe, this must be done only after the appropriate shader mode has been set
    //       as this function will bind the fetch shader
    void bind() const;

private:
    VertexBuffer*   mpVertexBuffer[VertexBuffer::NUM_MAX_BUFFERS];  // Vertex buffers
#if RIO_IS_CAFE
    u8              mFetchShader[0x20];                             // GX2FetchShader
    u8*             mpFetchShaderBuf;                               // Fetch shader buffer
    u32             mFetchShaderBufSize;                            // Fetch shader buffer size
#elif RIO_IS_WIN
    u32             mHandle;                                        // OpenGL handle
#endif
};

}

#endif // RIO_GPU_VERTEX_ARRAY_H
