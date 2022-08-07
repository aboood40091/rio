#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_VertexArray.h>

#include <GL/glew.h>

namespace rio {

VertexArray::~VertexArray()
{
    if (mHandle != GL_NONE)
    {
        glDeleteVertexArrays(1, &mHandle);
        mHandle = GL_NONE;
    }
}

void VertexArray::initialize()
{
    if (mHandle != GL_NONE)
    {
        glDeleteVertexArrays(1, &mHandle);
        mHandle = GL_NONE;
    }

    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr && !vb->mStreams.isEmpty())
            vb->mStreams.clear();
    }

    std::memset(mpVertexBuffer, 0, sizeof(VertexBuffer*) * VertexBuffer::NUM_MAX_BUFFERS);

    glGenVertexArrays(1, &mHandle);
    RIO_ASSERT(mHandle != GL_NONE);
}

void VertexArray::process()
{
    glBindVertexArray(mHandle);

    for (u32 i = 0; i < VertexBuffer::NUM_MAX_BUFFERS; i++)
    {
        VertexBuffer* vb = mpVertexBuffer[i];
        if (vb != nullptr && !vb->mStreams.isEmpty())
        {
            glBindBuffer(GL_ARRAY_BUFFER, vb->mHandle);

            RIO_ASSERT(vb->mStride != 0);
            u32 stride = vb->mStride;

            for (VertexStream::List::iterator it = vb->mStreams.begin(); it != vb->mStreams.end(); ++it)
            {
                VertexStream* stream = *it;

                RIO_ASSERT(stream->mFormat != VertexStream::FORMAT_INVALID);

                glEnableVertexAttribArray(stream->mLocation);
                if (stream->mInternalFormat.integer)
                {
                    glVertexAttribIPointer(stream->mLocation,
                                           stream->mInternalFormat.elem_count,
                                           stream->mInternalFormat.type,
                                           stride,
                                           (void*)stream->mOffset);
                }
                else
                {
                    glVertexAttribPointer(stream->mLocation,
                                          stream->mInternalFormat.elem_count,
                                          stream->mInternalFormat.type,
                                          stream->mInternalFormat.normalized,
                                          stride,
                                          (void*)stream->mOffset);
                }
            }
        }
    }

    glBindVertexArray(GL_NONE);
}

void VertexArray::bind() const
{
    glBindVertexArray(mHandle);
}

}

#endif // RIO_IS_WIN
