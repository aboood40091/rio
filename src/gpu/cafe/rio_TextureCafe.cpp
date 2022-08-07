#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <filedevice/rio_FileDeviceMgr.h>
#include <gpu/rio_Texture.h>

#include <gfd.h>
#include <gx2/mem.h>

namespace rio {

Texture2D::Texture2D(const char* base_fname)
    : mSelfAllocated(true)
{
    FileDevice::LoadArg arg;
    arg.path = std::string("textures/") + base_fname + ".gtx";

    u8* const file = FileDeviceMgr::instance()->load(arg);
    load_(file, arg.read_size);
    MemUtil::free(file);
}

void Texture2D::load_(const u8* file, u32)
{
    u32 alignment = GFDGetTextureAlignmentSize(0, file);

    u32 imageSize = GFDGetTextureImageSize(0, file);
    void* image = imageSize ? MemUtil::alloc(imageSize, alignment) : nullptr;

    u32 mipSize = GFDGetTextureMipImageSize(0, file);
    void* mipmaps = mipSize ? MemUtil::alloc(mipSize, alignment) : nullptr;

    GFDGetTexture(&mTextureInner,
                  image,
                  mipmaps,
                  0,
                  file);

    RIO_ASSERT(mTextureInner.surface.imageSize == imageSize);
    if (imageSize)
    {
        RIO_ASSERT(image);
        RIO_ASSERT((uintptr_t)image % alignment == 0);

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE, image, imageSize);
    }

    RIO_ASSERT(mTextureInner.surface.mipmapSize == mipSize);
    if (mipSize)
    {
        RIO_ASSERT(mipmaps);
        RIO_ASSERT((uintptr_t)mipmaps % alignment == 0);

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU_TEXTURE, mipmaps, mipSize);
    }

    mTextureInner.surface.image = image;
    mTextureInner.surface.mipmaps = mipmaps;

    createHandle_();
}

void Texture2D::createHandle_()
{
    RIO_ASSERT(mTextureInner.surface.image);
    mHandle = &mTextureInner;
}

Texture2D::~Texture2D()
{
    if (mSelfAllocated)
    {
        if (mTextureInner.surface.image)
        {
            MemUtil::free(mTextureInner.surface.image);
            mTextureInner.surface.image = nullptr;
        }

        if (mTextureInner.surface.mipmaps)
        {
            MemUtil::free(mTextureInner.surface.mipmaps);
            mTextureInner.surface.mipmaps = nullptr;
        }
    }
}

}

#endif // RIO_IS_CAFE
