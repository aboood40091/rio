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

Texture2D::Texture2D(TextureFormat format, u32 width, u32 height, u32 numMips)
    : mSelfAllocated(true)
{
    mTextureInner.surface.dim = GX2_SURFACE_DIM_TEXTURE_2D;
    mTextureInner.surface.width = width;
    mTextureInner.surface.height = height;
    mTextureInner.surface.depth = 1;
    mTextureInner.surface.mipLevels = numMips;
    mTextureInner.surface.format = (GX2SurfaceFormat)format;
    mTextureInner.surface.aa = GX2_AA_MODE1X;
    mTextureInner.surface.use = GX2_SURFACE_USE_TEXTURE;
    mTextureInner.surface.tileMode = GX2_TILE_MODE_DEFAULT;
    mTextureInner.surface.swizzle = 0;
    GX2CalcSurfaceSizeAndAlignment(&mTextureInner.surface);

    mTextureInner.viewFirstMip = 0;
    mTextureInner.viewNumMips = numMips;
    mTextureInner.viewFirstSlice = 0;
    mTextureInner.viewNumSlices = 1;
    mTextureInner.compMap = TextureFormatUtil::getDefaultCompMap(format);
    GX2InitTextureRegs(&mTextureInner);

    u32 alignment = mTextureInner.surface.alignment;

    u32 imageSize = mTextureInner.surface.imageSize;
    void* image = imageSize ? MemUtil::alloc(imageSize, alignment) : nullptr;

    u32 mipSize = mTextureInner.surface.mipmapSize;
    void* mipmaps = mipSize ? MemUtil::alloc(mipSize, alignment) : nullptr;

    if (imageSize)
    {
        RIO_ASSERT(image);
        RIO_ASSERT((uintptr_t)image % alignment == 0);

        GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, image, imageSize);
    }

    RIO_ASSERT(mTextureInner.surface.mipmapSize == mipSize);
    if (mipSize)
    {
        RIO_ASSERT(mipmaps);
        RIO_ASSERT((uintptr_t)mipmaps % alignment == 0);

        GX2Invalidate(GX2_INVALIDATE_MODE_TEXTURE, mipmaps, mipSize);
    }

    mTextureInner.surface.image = image;
    mTextureInner.surface.mipmaps = mipmaps;

    createHandle_();
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

void Texture2D::setCompMap(u32 compMap)
{
    mTextureInner.compMap = compMap;
    GX2InitTextureRegs(&mTextureInner);
}

}

#endif // RIO_IS_CAFE
