#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_UniformBlock.h>

#include <GL/glew.h>

namespace rio {

UniformBlock::UniformBlock(ShaderStage stage, u32 binding)
    : mBinding(binding)
    , mpData(nullptr)
    , mSize(0)
    , mStage(stage)
{
    glGenBuffers(1, &mHandle);
    RIO_ASSERT(mHandle != GL_NONE);
}

UniformBlock::~UniformBlock()
{
    if (mHandle != GL_NONE)
    {
        glDeleteBuffers(1, &mHandle);
        mHandle = GL_NONE;
    }
}

void UniformBlock::setData(const void* data, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);

    glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, mBinding, mHandle);

    if (size == mSize)
        glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);

    else
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);

    mpData = data;
    mSize = size;
}

void UniformBlock::setSubData(const void* data, u32 offset, u32 size)
{
    RIO_ASSERT(data != nullptr);
    RIO_ASSERT(size != 0);
    RIO_ASSERT(offset + size <= mSize);

    glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, mBinding, mHandle);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBlock::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
    glBindBufferBase(GL_UNIFORM_BUFFER, mBinding, mHandle);
}

}

#endif // RIO_IS_WIN
