#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_VertexArray.h>

#include <coreinit/memdefaultheap.h>
#include <gx2/draw.h>
#include <gx2/mem.h>
#include <gx2/shaders.h>
#include <gx2/utils.h>

#define GX2_COMP_SEL_X001 GX2_SEL_MASK(GX2_SQ_SEL_X, GX2_SQ_SEL_0, GX2_SQ_SEL_0, GX2_SQ_SEL_1)
#define GX2_COMP_SEL_XY01 GX2_SEL_MASK(GX2_SQ_SEL_X, GX2_SQ_SEL_Y, GX2_SQ_SEL_0, GX2_SQ_SEL_1)
#define GX2_COMP_SEL_XYZ1 GX2_SEL_MASK(GX2_SQ_SEL_X, GX2_SQ_SEL_Y, GX2_SQ_SEL_Z, GX2_SQ_SEL_1)
#define GX2_COMP_SEL_XYZW GX2_SEL_MASK(GX2_SQ_SEL_X, GX2_SQ_SEL_Y, GX2_SQ_SEL_Z, GX2_SQ_SEL_W)

namespace rio {

VertexArray::~VertexArray()
{
    if (mpFetchShaderBuf)
    {
        delete[] (u8*)mpFetchShaderBuf;
        mpFetchShaderBuf = nullptr;
    }
}

void VertexArray::initialize()
{
    if (mpFetchShaderBuf)
    {
        delete[] (u8*)mpFetchShaderBuf;
        mpFetchShaderBuf = nullptr;
    }

    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr && !vb->mStreams.isEmpty())
            vb->mStreams.clear();
    }

    std::memset(mpVertexBuffer, 0, sizeof(VertexBuffer*) * VertexBuffer::NUM_MAX_BUFFERS);
}

void VertexArray::process()
{
    static constexpr u32 sFormatMask[] = {
        GX2_COMP_SEL_X001, GX2_COMP_SEL_XY01,
        GX2_COMP_SEL_X001, GX2_COMP_SEL_X001,
        GX2_COMP_SEL_XY01, GX2_COMP_SEL_X001,
        GX2_COMP_SEL_X001, GX2_COMP_SEL_XY01,
        GX2_COMP_SEL_XY01, GX2_COMP_SEL_XYZ1,
        GX2_COMP_SEL_XYZW, GX2_COMP_SEL_XYZW,
        GX2_COMP_SEL_XY01, GX2_COMP_SEL_XY01,
        GX2_COMP_SEL_XYZW, GX2_COMP_SEL_XYZW,
        GX2_COMP_SEL_XYZ1, GX2_COMP_SEL_XYZ1,
        GX2_COMP_SEL_XYZW, GX2_COMP_SEL_XYZW
    };

    u32 num_streams = 0;
    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr)
            num_streams += vb->mStreams.size();
    }

    GX2AttribStream* streams = new GX2AttribStream[num_streams];

    u32 j = 0;
    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr && !vb->mStreams.isEmpty())
        {
            u32 buffer = vb->mBuffer;

            for (VertexStream::List::iterator it = vb->mStreams.begin(); it != vb->mStreams.end(); ++it)
            {
                VertexStream* stream = *it;
                GX2AttribStream& gx2_stream = streams[j++];
                gx2_stream.location = stream->mLocation;
                gx2_stream.buffer = buffer;
                gx2_stream.offset = stream->mOffset;
                gx2_stream.format = (GX2AttribFormat)stream->mInternalFormat;
                gx2_stream.mask = sFormatMask[stream->mInternalFormat & 0xff];
                gx2_stream.endianSwap = GX2_ENDIAN_SWAP_DEFAULT;
                // TODO
                gx2_stream.type = GX2_ATTRIB_INDEX_PER_VERTEX;
                gx2_stream.aluDivisor = 0;
            }
        }
    }
    RIO_ASSERT(j == num_streams);

    mFetchShaderBufSize = GX2CalcFetchShaderSizeEx(
        num_streams,
        GX2_FETCH_SHADER_TESSELLATION_NONE, // No Tessellation
        GX2_TESSELLATION_MODE_DISCRETE      // ^^^^^^^^^^^^^^^
    );
    mpFetchShaderBuf = (u8*)MEMAllocFromDefaultHeapEx(
        mFetchShaderBufSize,
        GX2_SHADER_PROGRAM_ALIGNMENT
    );

    GX2FetchShader* fetch_shader = (GX2FetchShader*)mFetchShader;

    GX2InitFetchShaderEx(
        fetch_shader,
        mpFetchShaderBuf,
        num_streams,
        streams,
        GX2_FETCH_SHADER_TESSELLATION_NONE, // No Tessellation
        GX2_TESSELLATION_MODE_DISCRETE      // ^^^^^^^^^^^^^^^
    );

    // Make sure to flush CPU cache and invalidate GPU cache
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, fetch_shader->program, fetch_shader->size);
}

void VertexArray::bind() const
{
    RIO_ASSERT(mpFetchShaderBuf != nullptr);
    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr)
        {
            RIO_ASSERT(vb->mStride != 0);
            GX2SetAttribBuffer(vb->mBuffer, vb->mSize, vb->mStride, vb->mpData);
        }
    }
    GX2SetFetchShader((GX2FetchShader*)mFetchShader);
}

}

#endif // RIO_IS_CAFE
