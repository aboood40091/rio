#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <filedevice/rio_FileDeviceMgr.h>
#include <gpu/rio_Texture.h>

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

static const GLint TEX_COMP_MAP_TO_GL[6] = {
    GL_RED, GL_GREEN, GL_BLUE,
    GL_ALPHA, GL_ZERO, GL_ONE
};

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
    NativeSurface2D& surface = mTextureInner.surface;

    glGenTextures(1, &mHandle);
    RIO_ASSERT(mHandle != GL_NONE);
    glBindTexture(GL_TEXTURE_2D, mHandle);

    GLint compMap[4] = {
        TEX_COMP_MAP_TO_GL[mTextureInner.compMap >> 24 & 0xFF],
        TEX_COMP_MAP_TO_GL[mTextureInner.compMap >> 16 & 0xFF],
        TEX_COMP_MAP_TO_GL[mTextureInner.compMap >>  8 & 0xFF],
        TEX_COMP_MAP_TO_GL[mTextureInner.compMap >>  0 & 0xFF]
    };

    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, compMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, surface.mipLevels - 1);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch (surface.format)
    {
    case TEXTURE_FORMAT_BC1_UNORM:
    case TEXTURE_FORMAT_BC2_UNORM:
    case TEXTURE_FORMAT_BC3_UNORM:
    case TEXTURE_FORMAT_BC4_UNORM:
    case TEXTURE_FORMAT_BC4_SNORM:
    case TEXTURE_FORMAT_BC5_UNORM:
    case TEXTURE_FORMAT_BC5_SNORM:
        {
            glCompressedTexImage2D(GL_TEXTURE_2D,
                                   0,
                                   surface.nativeFormat.internalformat,
                                   surface.width,
                                   surface.height,
                                   0,
                                   surface.imageSize,
                                   surface.image);

            if (surface.mipLevels > 1)
            {
                for (u32 i = 0; i < surface.mipLevels - 2; i++)
                    glCompressedTexImage2D(GL_TEXTURE_2D,
                                           i + 1,
                                           surface.nativeFormat.internalformat,
                                           std::max(surface.width  >> (i + 1), 1u),
                                           std::max(surface.height >> (i + 1), 1u),
                                           0,
                                           surface.mipLevelOffset[i + 1] - surface.mipLevelOffset[i],
                                           (u8*)surface.mipmaps + surface.mipLevelOffset[i]);

                glCompressedTexImage2D(GL_TEXTURE_2D,
                                       (surface.mipLevels - 2) + 1,
                                       surface.nativeFormat.internalformat,
                                       std::max(surface.width  >> ((surface.mipLevels - 2) + 1), 1u),
                                       std::max(surface.height >> ((surface.mipLevels - 2) + 1), 1u),
                                       0,
                                       surface.mipmapSize - surface.mipLevelOffset[(surface.mipLevels - 2)],
                                       (u8*)surface.mipmaps + surface.mipLevelOffset[(surface.mipLevels - 2)]);
            }
        }
        break;
    case TEXTURE_FORMAT_BC1_SRGB:
    case TEXTURE_FORMAT_BC2_SRGB:
    case TEXTURE_FORMAT_BC3_SRGB:
        {
            glCompressedTexImage2DARB(GL_TEXTURE_2D,
                                      0,
                                      surface.nativeFormat.internalformat,
                                      surface.width,
                                      surface.height,
                                      0,
                                      surface.imageSize,
                                      surface.image);

            if (surface.mipLevels > 1)
            {
                for (u32 i = 0; i < surface.mipLevels - 2; i++)
                    glCompressedTexImage2DARB(GL_TEXTURE_2D,
                                              i + 1,
                                              surface.nativeFormat.internalformat,
                                              std::max(surface.width  >> (i + 1), 1u),
                                              std::max(surface.height >> (i + 1), 1u),
                                              0,
                                              surface.mipLevelOffset[i + 1] - surface.mipLevelOffset[i],
                                              (u8*)surface.mipmaps + surface.mipLevelOffset[i]);

                glCompressedTexImage2DARB(GL_TEXTURE_2D,
                                          (surface.mipLevels - 2) + 1,
                                          surface.nativeFormat.internalformat,
                                          std::max(surface.width  >> ((surface.mipLevels - 2) + 1), 1u),
                                          std::max(surface.height >> ((surface.mipLevels - 2) + 1), 1u),
                                          0,
                                          surface.mipmapSize - surface.mipLevelOffset[(surface.mipLevels - 2)],
                                          (u8*)surface.mipmaps + surface.mipLevelOffset[(surface.mipLevels - 2)]);
            }
        }
        break;
    default:
        {
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         surface.nativeFormat.internalformat,
                         surface.width,
                         surface.height,
                         0,
                         surface.nativeFormat.format,
                         surface.nativeFormat.type,
                         surface.image);

            if (surface.mipLevels > 1)
            {
                for (u32 i = 0; i < surface.mipLevels - 2; i++)
                    glTexImage2D(GL_TEXTURE_2D,
                                 i + 1,
                                 surface.nativeFormat.internalformat,
                                 std::max(surface.width  >> (i + 1), 1u),
                                 std::max(surface.height >> (i + 1), 1u),
                                 0,
                                 surface.nativeFormat.format,
                                 surface.nativeFormat.type,
                                 (u8*)surface.mipmaps + surface.mipLevelOffset[i]);

                glTexImage2D(GL_TEXTURE_2D,
                             (surface.mipLevels - 2) + 1,
                             surface.nativeFormat.internalformat,
                             std::max(surface.width  >> ((surface.mipLevels - 2) + 1), 1u),
                             std::max(surface.height >> ((surface.mipLevels - 2) + 1), 1u),
                             0,
                             surface.nativeFormat.format,
                             surface.nativeFormat.type,
                             (u8*)surface.mipmaps + surface.mipLevelOffset[(surface.mipLevels - 2)]);
            }
        }
        break;
    }
}

Texture2D::~Texture2D()
{
    if (mHandle != GL_NONE)
    {
        glDeleteTextures(1, &mHandle);
        mHandle = GL_NONE;

        if (mSelfAllocated)
        {
            // Free file data
            MemUtil::free((u8*)mTextureInner.surface.image - mTextureInner.surface._imageOffset);
        }
    }
}

}

#endif // RIO_IS_WIN
