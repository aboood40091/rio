#ifndef RIO_GPU_UNIFORM_BLOCK_H
#define RIO_GPU_UNIFORM_BLOCK_H

#include <misc/rio_Types.h>

namespace rio {

class UniformBlock
{
    // Only for uniform blocks with std140 layout.
    // Explicit binding on Windows with OpenGL is currently NOT supported.

    // Note: On Cafe, data is passed directly to the GPU, therefore it must not be
    //       freed as long as this uniform block buffer is being used.

    //       Furthermore, keep in mind that on Cafe the data setter functions byteswap the passed data!
    //       Cafe's GPU7 expects uniform data to be in little endian, whereas the CPU is big endian.

    //       Also, on Cafe, uniform blocks can have different indices between the Vertex and Fragment shaders.
    //       They can only guaranteed to be the same through explicit binding in the shader sources.
    //       As for Windows, which currently uses OpenGL, it is guaranteed that the uniform block index
    //       will be the same for the Vertex and Fragment shaders.

    // TODO: Buffer self-allocation function to avoid redundant byteswap of
    //       empty passed data buffer (that is later filled using setSubData() calls) on Cafe.

public:
    enum ShaderStage : u32
    {
        STAGE_NONE              = 0,
        STAGE_VERTEX_SHADER     = (1 << 0),
        STAGE_FRAGMENT_SHADER   = (1 << 1),
        STAGE_ALL               = STAGE_VERTEX_SHADER | STAGE_FRAGMENT_SHADER
    };

public:
    UniformBlock(ShaderStage stage = STAGE_ALL, u32 index = u32(-1));
    UniformBlock(ShaderStage stage, u32 vs_index, u32 fs_index);
    ~UniformBlock();

private:
    UniformBlock(const UniformBlock&);
    UniformBlock& operator=(const UniformBlock&);

public:
    UniformBlock(const void* data, u32 size, ShaderStage stage = STAGE_ALL, u32 index = u32(-1))
        : UniformBlock(stage, index)
    {
        setDataInvalidate(data, size);
    }

    UniformBlock(const void* data, u32 size, ShaderStage stage, u32 vs_index, u32 fs_index)
        : UniformBlock(stage, vs_index, fs_index)
    {
        setDataInvalidate(data, size);
    }
    const void* getData() const { return mpData; }
    u32 getSize() const { return mSize; }
    ShaderStage getStage() const { return mStage; }

    u32 getVSIndex() const { return mVSIndex; }
    u32 getFSIndex() const { return mFSIndex; }

    void setIndex(u32 index)
    {
        mVSIndex = index;
        mFSIndex = index;
    }

    void setIndex(u32 vs_index, u32 fs_index)
    {
        mVSIndex = vs_index;
        mFSIndex = fs_index;
    }

    void setVSIndex(u32 vs_index)
    {
        mVSIndex = vs_index;
    }

    void setFSIndex(u32 fs_index)
    {
        mFSIndex = fs_index;
    }

    void setStage(ShaderStage stage)
    {
        mStage = stage;
    }

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
    u32         mVSIndex; // Buffer index in Vertex shader
    u32         mFSIndex; // Buffer index in Fragment shader
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
