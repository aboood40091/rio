#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_UniformBlock.h>
#include <misc/rio_MemUtil.h>

#include <misc/gl/rio_GL.h>

namespace rio {

UniformBlock::UniformBlock(ShaderStage stage, u32 index)
    : mVSIndex(index)
    , mFSIndex(index)
    , mpData(nullptr)
    , mSize(0)
    , mStage(stage)
{
    RIO_GL_CALL(glGenBuffers(1, &mHandle));
    RIO_ASSERT(mHandle != GL_NONE);
}

UniformBlock::UniformBlock(ShaderStage stage, u32 vs_index, u32 fs_index)
    : mVSIndex(vs_index)
    , mFSIndex(fs_index)
    , mpData(nullptr)
    , mSize(0)
    , mStage(stage)
{
    RIO_ASSERT(mVSIndex == mFSIndex);

    RIO_GL_CALL(glGenBuffers(1, &mHandle));
    RIO_ASSERT(mHandle != GL_NONE);
}

UniformBlock::~UniformBlock()
{
    if (mHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteBuffers(1, &mHandle));
        mHandle = GL_NONE;
    }
}

void UniformBlock::setData(const void* data, u32 size)
{
    RIO_ASSERT(size != 0);

    RIO_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, mHandle));

    if (size == mSize)
    {
        if (data != nullptr)
            RIO_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data));
    }
    else
    {
        RIO_GL_CALL(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }

    mpData = data;
    mSize = size;
}

void UniformBlock::setSubData(const void* data, u32 offset, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(offset + size <= mSize);

    if (mpData != nullptr)
    {
        const uintptr_t dst = uintptr_t(mpData) + offset;
        const uintptr_t src = uintptr_t(data);
        if (src != dst)
            MemUtil::copy((void*)dst, (const void*)src, size);
    }

    RIO_GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, mHandle));

    RIO_GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
}

void UniformBlock::bind() const
{
    if (!mStage)
        return;

    u32 index;
    if (mStage & STAGE_ALL)
    {
        RIO_ASSERT(mVSIndex == mFSIndex);
        index = mVSIndex;
    }
    else if (mStage & STAGE_VERTEX_SHADER)
    {
        index = mVSIndex;
    }
    else // if (mStage & STAGE_FRAGMENT_SHADER)
    {
        index = mFSIndex;
    }

    RIO_ASSERT(index != GL_INVALID_INDEX);

    RIO_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, index, mHandle));
}

}

#endif // RIO_IS_WIN
