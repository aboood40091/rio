#ifndef RIO_GPU_VERTEX_STREAM_H
#define RIO_GPU_VERTEX_STREAM_H

#include <container/rio_TList.h>

namespace rio {

class VertexArray;
class VertexBuffer;

class VertexStream : private TListNode<VertexStream*>
{
    // Wrapper class representing the layout of a vertex attribute stream

public:
    enum Format
    {
        // U = Unsigned
        // S = Signed
        // NORM = Normalized to float
        // INT = Integer
        // INT_TO_FLOAT = Direct conversion from integer to float
        // FLOAT = Floating-point

        FORMAT_INVALID = 0,

        FORMAT_8_UNORM,
        FORMAT_8_8_UNORM,
        FORMAT_8_8_8_8_UNORM,

        FORMAT_8_UINT,
        FORMAT_8_8_UINT,
        FORMAT_8_8_8_8_UINT,

        FORMAT_8_UINT_TO_FLOAT,
        FORMAT_8_8_UINT_TO_FLOAT,
        FORMAT_8_8_8_8_UINT_TO_FLOAT,

        FORMAT_8_SNORM,
        FORMAT_8_8_SNORM,
        FORMAT_8_8_8_8_SNORM,

        FORMAT_8_SINT,
        FORMAT_8_8_SINT,
        FORMAT_8_8_8_8_SINT,

        FORMAT_8_SINT_TO_FLOAT,
        FORMAT_8_8_SINT_TO_FLOAT,
        FORMAT_8_8_8_8_SINT_TO_FLOAT,

        FORMAT_10_11_11_FLOAT,

        FORMAT_10_10_10_2_UNORM,

        FORMAT_10_10_10_2_UINT,

        FORMAT_10_10_10_2_SNORM,

        FORMAT_10_10_10_2_SINT,

        FORMAT_16_UNORM,
        FORMAT_16_16_UNORM,
        FORMAT_16_16_16_16_UNORM,

        FORMAT_16_UINT,
        FORMAT_16_16_UINT,
        FORMAT_16_16_16_16_UINT,

        FORMAT_16_UINT_TO_FLOAT,
        FORMAT_16_16_UINT_TO_FLOAT,
        FORMAT_16_16_16_16_UINT_TO_FLOAT,

        FORMAT_16_SNORM,
        FORMAT_16_16_SNORM,
        FORMAT_16_16_16_16_SNORM,

        FORMAT_16_SINT,
        FORMAT_16_16_SINT,
        FORMAT_16_16_16_16_SINT,

        FORMAT_16_SINT_TO_FLOAT,
        FORMAT_16_16_SINT_TO_FLOAT,
        FORMAT_16_16_16_16_SINT_TO_FLOAT,

        FORMAT_16_FLOAT,
        FORMAT_16_16_FLOAT,
        FORMAT_16_16_16_16_FLOAT,

        FORMAT_32_UINT,
        FORMAT_32_32_UINT,
        FORMAT_32_32_32_UINT,
        FORMAT_32_32_32_32_UINT,

        FORMAT_32_SINT,
        FORMAT_32_32_SINT,
        FORMAT_32_32_32_SINT,
        FORMAT_32_32_32_32_SINT,

        FORMAT_32_FLOAT,
        FORMAT_32_32_FLOAT,
        FORMAT_32_32_32_FLOAT,
        FORMAT_32_32_32_32_FLOAT
    };

private:
    typedef TList<VertexStream*> List;

#if RIO_IS_WIN

    struct InternalFormat
    {
        u8      elem_count;
        bool    normalized;
        bool    integer;
        u32     type;
    };

#else

    typedef u32 InternalFormat;

#endif

public:
    VertexStream()
        : TListNode<VertexStream*>(this)
        , mInternalFormat()
        , mFormat(FORMAT_INVALID)
    {
    }

    VertexStream(u8 location, Format format, u32 offset)
        : TListNode<VertexStream*>(this)
    {
        setLayout(location, format, offset);
    }

    // Set layout of vertex attribute stream
    void setLayout(u8 location, Format format, u32 offset);

private:
    u8              mLocation;          // Shader location
    InternalFormat  mInternalFormat;    // Native format
    Format          mFormat;            // Format
    u32             mOffset;            // Offset to data in vertex buffer
    VertexBuffer*   mpVertexBuffer;     // Vertex buffer

    friend class VertexArray;
    friend class VertexBuffer;
};

}

#endif // RIO_GPU_VERTEX_STREAM_H
