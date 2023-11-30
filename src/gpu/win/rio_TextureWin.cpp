#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <filedevice/rio_FileDeviceMgr.h>
#include <gpu/rio_Texture.h>
#include <gpu/win/rio_Texture2DUtilWin.h>

#include <algorithm>

namespace {

//------------------------------------------------------------

// RTX: Custom texture format for this API
// Magic: 5101382D; "RIOTEX2D"
static const u32 TEX_MAGIC = 0x5101382D;

// Minimum supported version
static const u32 TEX_VERSION_MIN     = 0x01000000;
// Current version
static const u32 TEX_VERSION_CURRENT = 0x01000000;

// Structure size
static const size_t TEX_SIZE = sizeof(rio::NativeTexture2D);

//------------------------------------------------------------

}

namespace rio {

Texture2D::Texture2D(const char* base_fname)
    : mSelfAllocated(true)
{
    FileDevice::LoadArg arg;
    arg.path = std::string("textures/") + base_fname + ".rtx";

    u8* const file = FileDeviceMgr::instance()->load(arg);
    load_(file, arg.read_size);
    MemUtil::free(file);
}

Texture2D::Texture2D(TextureFormat format, u32 width, u32 height, u32 numMips)
    : mSelfAllocated(false)
{
    NativeSurface2D& surface = mTextureInner.surface;
    surface.width = width;
    surface.height = height;
    surface.mipLevels = numMips;
    surface.format = format;

    [[maybe_unused]] bool success = TextureFormatUtil::getNativeTextureFormat(surface.nativeFormat, format);
    RIO_ASSERT(success);

    surface.imageSize = Texture2DUtil::calcImageSize(format, width, height);
    surface.mipmapSize = Texture2DUtil::calcMipmapSize(format, width, height, numMips, surface.mipLevelOffset);
    surface._imageOffset = 0;
    surface._mipmapsOffset = 0;
    surface.image = nullptr;
    surface.mipmaps = nullptr;

    mTextureInner.compMap = TextureFormatUtil::getDefaultCompMap(format);

    {
        mTextureInner._footer.magic = 0x5101382D;
        mTextureInner._footer.version = TEX_VERSION_CURRENT;
    }

    createHandle_();
}

void Texture2D::load_(const u8* file, u32 file_size)
{
    RIO_ASSERT(file_size >= TEX_SIZE);

    NativeTexture2D* tex = (NativeTexture2D*)MemUtil::alloc(file_size, 4);
    RIO_ASSERT(tex);

    MemUtil::copy(tex, file, file_size);

    RIO_ASSERT(tex->_footer.magic == 0x5101382D);
    RIO_ASSERT(TEX_VERSION_MIN <= tex->_footer.version);
    RIO_ASSERT(tex->_footer.version <= TEX_VERSION_CURRENT);

    mTextureInner = *tex;
    NativeSurface2D& surface = mTextureInner.surface;

    surface.image = (u8*)tex + surface._imageOffset;

    if (surface.mipLevels > 1)
        surface.mipmaps = (u8*)tex + surface._mipmapsOffset;

    else
        surface.mipmaps = nullptr;

    createHandle_();
}

void Texture2D::createHandle_()
{
    RIO_ASSERT(mTextureInner._footer.magic == 0x5101382D);
    RIO_ASSERT(TEX_VERSION_MIN <= mTextureInner._footer.version);
    RIO_ASSERT(mTextureInner._footer.version <= TEX_VERSION_CURRENT);

    NativeSurface2D& surface = mTextureInner.surface;

    mHandle = Texture2DUtil::createHandle(
        surface.format,
        surface.nativeFormat,
        surface.width,
        surface.height,
        surface.mipLevels,
        surface.imageSize,
        surface.image,
        surface.mipmapSize,
        surface.mipmaps,
        surface.mipLevelOffset,
        mTextureInner.compMap
    );
    RIO_ASSERT(mHandle != GL_NONE);
}

Texture2D::~Texture2D()
{
    if (mHandle != GL_NONE)
    {
        Texture2DUtil::destroyHandle(mHandle);
        mHandle = GL_NONE;

        if (mSelfAllocated)
        {
            // Free file data
            MemUtil::free((u8*)mTextureInner.surface.image - mTextureInner.surface._imageOffset);
        }
    }
}

void Texture2D::setCompMap(u32 compMap)
{
    mTextureInner.compMap = compMap;
    Texture2DUtil::setSwizzle(mHandle, compMap);
}

}

#endif // RIO_IS_WIN
