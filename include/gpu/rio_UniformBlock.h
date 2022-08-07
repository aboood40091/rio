#ifndef RIO_GPU_UNIFORM_BLOCK_H
#define RIO_GPU_UNIFORM_BLOCK_H

#include <misc/rio_Types.h>

namespace rio {

class UniformBlock
{
    // Only for uniform blocks with std140 layout and explicit binding.

    // Note: On Cafe, data is passed directly to the GPU, therefore it must not be
    //       freed as long as this uniform block buffer is being used.

    //       Furthermore, keep in mind that the data setter functions byteswap the passed data!
    //       Cafe's GPU7 expects uniform data to be in little endian, whereas the CPU is big endian.

    // TODO: Buffer self-allocation function to avoid redundant byteswap of
    //       empty passed data buffer (that is later filled using setSubData() calls) on Cafe.

public:
    enum ShaderStage : u32
    {
        STAGE_VERTEX_SHADER   = (1 << 0),
        STAGE_FRAGMENT_SHADER = (1 << 1),
        STAGE_ALL             = STAGE_VERTEX_SHADER | STAGE_FRAGMENT_SHADER
    };

public:
    UniformBlock(ShaderStage stage, u32 binding);
    ~UniformBlock();

private:
    UniformBlock(const UniformBlock&);
    UniformBlock& operator=(const UniformBlock&);

public:
    UniformBlock(const void* data, u32 size, ShaderStage stage, u32 binding)
        : UniformBlock(stage, binding)
    {
        setDataInvalidate(data, size);
    }

    const void* getData() const { return mpData; }
    u32 getSize() const { return mSize; }
    ShaderStage getStage() const { return mStage; }

    // Sets the passed data pointer as this object's data buffer. (Byteswaps data on Cafe, therefore the passed data *source* is affected too!)
    void setData(const void* data, u32 size);
    // Copies the passed data into the specified range. (Byteswaps data on Cafe, but the passed data *source* is NOT affected.)
    void setSubData(const void* data, u32 offset, u32 size);

    // Setter functions with cache invalidation (same rules as above apply, currently only useful for Cafe):
    void setDataInvalidate(const void* data, u32 size);
    void setSubDataInvalidate(const void* data, u32 offset, u32 size);

    static void invalidateCache(const void* data, u32 size);

    void bind() const;

private:
    u32         mBinding;   // Buffer binding point
#if RIO_IS_WIN
    u32         mHandle;    // Buffer handle (for OpenGL)
#endif // RIO_IS_WIN
    const void* mpData;     // Buffer data
    u32         mSize;      // Buffer size
    ShaderStage mStage;     // Shader stage
};

#if RIO_IS_WIN

inline void UniformBlock::setDataInvalidate(const void* data, u32 size)
{
    setData(data, size);
}

inline void UniformBlock::setSubDataInvalidate(const void* data, u32 offset, u32 size)
{
    setSubData(data, offset, size);
}

inline void UniformBlock::invalidateCache(const void* data, u32 size)
{
}

#endif // RIO_IS_WIN

}

#endif // RIO_GPU_UNIFORM_BLOCK_H
