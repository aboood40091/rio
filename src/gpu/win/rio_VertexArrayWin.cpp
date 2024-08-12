#include <misc/rio_Types.h>

#if RIO_IS_DESKTOP

#include <gpu/rio_VertexArray.h>

#include <misc/gl/rio_GL.h>

namespace rio {

VertexArray::~VertexArray()
{
    if (mHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteVertexArrays(1, &mHandle));
        mHandle = GL_NONE;
    }
}

void VertexArray::initialize()
{
    if (mHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteVertexArrays(1, &mHandle));
        mHandle = GL_NONE;
    }

    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr && !vb->mStreams.isEmpty())
            vb->mStreams.clear();
    }

    std::memset(mpVertexBuffer, 0, sizeof(VertexBuffer*) * VertexBuffer::NUM_MAX_BUFFERS);

    RIO_GL_CALL(glGenVertexArrays(1, &mHandle));
    RIO_ASSERT(mHandle != GL_NONE);
}

void VertexArray::process()
{
    RIO_GL_CALL(glBindVertexArray(mHandle));

    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr && !vb->mStreams.isEmpty())
        {
            RIO_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vb->mHandle));

            RIO_ASSERT(vb->mStride != 0);
            u32 stride = vb->mStride;

            for (VertexStream::List::iterator it = vb->mStreams.begin(); it != vb->mStreams.end(); ++it)
            {
                VertexStream* stream = *it;

                RIO_ASSERT(stream->mFormat != VertexStream::FORMAT_INVALID);

                RIO_GL_CALL(glEnableVertexAttribArray(stream->mLocation));
                if (stream->mInternalFormat.integer)
                {
                    RIO_GL_CALL(glVertexAttribIPointer(
                        stream->mLocation,
                        stream->mInternalFormat.elem_count,
                        stream->mInternalFormat.type,
                        stride,
                        (void*)(uintptr_t)stream->mOffset
                    ));
                }
                else
                {
                    RIO_GL_CALL(glVertexAttribPointer(
                        stream->mLocation,
                        stream->mInternalFormat.elem_count,
                        stream->mInternalFormat.type,
                        stream->mInternalFormat.normalized,
                        stride,
                        (void*)(uintptr_t)stream->mOffset
                    ));
                }
            }
        }
    }

    RIO_GL_CALL(glBindVertexArray(GL_NONE));
}

void VertexArray::bind() const
{
    RIO_GL_CALL(glBindVertexArray(mHandle));
}

}

#endif // RIO_IS_DESKTOP
