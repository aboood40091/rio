#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/win/rio_Texture2DUtilWin.h>

#include <misc/rio_MemUtil.h>
#include <misc/gl/rio_GL.h>

#include <algorithm>

namespace rio {

u32 Texture2DUtil::calcImageSize(
    TextureFormat format,
    u32 width,
    u32 height
)
{
    u32 bpp = TextureFormatUtil::getPixelByteSize(format);

    if (TextureFormatUtil::isCompressed(format))
    {
        width = (width + 3) / 4;
        height = (height + 3) / 4;
    }

    return width * height * bpp;
}

u32 Texture2DUtil::calcMipmapSize(
    TextureFormat format,
    u32 width,
    u32 height,
    u32 mipLevels,
    u32* mipLevelOffset
)
{
    if (mipLevelOffset)
        MemUtil::set(mipLevelOffset, 0, sizeof(u32) * 13);

    mipLevels = std::min(std::max(mipLevels, 1u), 14u);

    u32 bpp = TextureFormatUtil::getPixelByteSize(format);

    u32 mipmapSize = 0;

    for (u32 i = 1; i < mipLevels; i++)
    {
        if (mipLevelOffset)
            mipLevelOffset[i - 1] = mipmapSize;

        u32 mipWidth  = std::max(width  >> i, 1u);
        u32 mipHeight = std::max(height >> i, 1u);

        if (TextureFormatUtil::isCompressed(format))
        {
            mipWidth = (mipWidth + 3) / 4;
            mipHeight = (mipHeight + 3) / 4;
        }

        mipmapSize += mipWidth * mipHeight * bpp;
    }

    return mipmapSize;
}

u32 Texture2DUtil::createHandle()
{
    u32 handle = GL_NONE;
    RIO_GL_CALL(glGenTextures(1, &handle));
    RIO_ASSERT(handle != GL_NONE);
    return handle;
}

void Texture2DUtil::destroyHandle(u32 handle)
{
    RIO_ASSERT(handle != GL_NONE);
    RIO_GL_CALL(glDeleteTextures(1, &handle));
}

void Texture2DUtil::bind(u32 handle)
{
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, handle));
}

void Texture2DUtil::setSwizzleCurrent(u32 compMap)
{
    static const GLint TEX_COMP_MAP_TO_GL[6] = {
        GL_RED, GL_GREEN, GL_BLUE,
        GL_ALPHA, GL_ZERO, GL_ONE
    };

    GLint swizzle[4] = {
        TEX_COMP_MAP_TO_GL[compMap >> 24 & 0xFF],
        TEX_COMP_MAP_TO_GL[compMap >> 16 & 0xFF],
        TEX_COMP_MAP_TO_GL[compMap >>  8 & 0xFF],
        TEX_COMP_MAP_TO_GL[compMap >>  0 & 0xFF]
    };

    RIO_GL_CALL(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle));
}

void Texture2DUtil::setNumMipsCurrent(u32 mipLevels)
{
    mipLevels = std::min(std::max(mipLevels, 1u), 14u);

    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevels - 1));
}

void Texture2DUtil::uploadTextureCurrent(
    TextureFormat format,
    const NativeTextureFormat& nativeFormat,
    u32 width,
    u32 height,
    u32 mipLevels,
    u32 imageSize,
    const void* image,
    u32 mipmapSize,
    const void* mipmaps,
    const u32* mipLevelOffset
)
{
    RIO_ASSERT(format != TEXTURE_FORMAT_INVALID);
    RIO_ASSERT(width && height);
    RIO_ASSERT(imageSize);

    mipLevels = std::min(std::max(mipLevels, 1u), 14u);

    if (mipLevels > 1)
    {
        RIO_ASSERT(mipmapSize);
        RIO_ASSERT(mipLevelOffset);
    }

    RIO_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    switch (format)
    {
    case TEXTURE_FORMAT_BC1_UNORM:
    case TEXTURE_FORMAT_BC2_UNORM:
    case TEXTURE_FORMAT_BC3_UNORM:
    case TEXTURE_FORMAT_BC4_UNORM:
    case TEXTURE_FORMAT_BC4_SNORM:
    case TEXTURE_FORMAT_BC5_UNORM:
    case TEXTURE_FORMAT_BC5_SNORM:
        {
            RIO_GL_CALL(glCompressedTexImage2D(
                GL_TEXTURE_2D,
                0,
                nativeFormat.internalformat,
                width,
                height,
                0,
                imageSize,
                image
            ));

            if (mipLevels > 1)
            {
                for (u32 i = 0; i < mipLevels - 2; i++)
                    RIO_GL_CALL(glCompressedTexImage2D(
                        GL_TEXTURE_2D,
                        i + 1,
                        nativeFormat.internalformat,
                        std::max(width  >> (i + 1), 1u),
                        std::max(height >> (i + 1), 1u),
                        0,
                        mipLevelOffset[i + 1] - mipLevelOffset[i],
                        mipmaps ? ((u8*)mipmaps + mipLevelOffset[i]) : nullptr
                    ));

                RIO_GL_CALL(glCompressedTexImage2D(
                    GL_TEXTURE_2D,
                    (mipLevels - 2) + 1,
                    nativeFormat.internalformat,
                    std::max(width  >> ((mipLevels - 2) + 1), 1u),
                    std::max(height >> ((mipLevels - 2) + 1), 1u),
                    0,
                    mipmapSize - mipLevelOffset[(mipLevels - 2)],
                    mipmaps ? ((u8*)mipmaps + mipLevelOffset[(mipLevels - 2)]) : nullptr
                ));
            }
        }
        break;
    case TEXTURE_FORMAT_BC1_SRGB:
    case TEXTURE_FORMAT_BC2_SRGB:
    case TEXTURE_FORMAT_BC3_SRGB:
        {
            RIO_GL_CALL(glCompressedTexImage2DARB(
                GL_TEXTURE_2D,
                0,
                nativeFormat.internalformat,
                width,
                height,
                0,
                imageSize,
                image
            ));

            if (mipLevels > 1)
            {
                for (u32 i = 0; i < mipLevels - 2; i++)
                    RIO_GL_CALL(glCompressedTexImage2DARB(
                        GL_TEXTURE_2D,
                        i + 1,
                        nativeFormat.internalformat,
                        std::max(width  >> (i + 1), 1u),
                        std::max(height >> (i + 1), 1u),
                        0,
                        mipLevelOffset[i + 1] - mipLevelOffset[i],
                        mipmaps ? ((u8*)mipmaps + mipLevelOffset[i]) : nullptr
                    ));

                RIO_GL_CALL(glCompressedTexImage2DARB(
                    GL_TEXTURE_2D,
                    (mipLevels - 2) + 1,
                    nativeFormat.internalformat,
                    std::max(width  >> ((mipLevels - 2) + 1), 1u),
                    std::max(height >> ((mipLevels - 2) + 1), 1u),
                    0,
                    mipmapSize - mipLevelOffset[(mipLevels - 2)],
                    mipmaps ? ((u8*)mipmaps + mipLevelOffset[(mipLevels - 2)]) : nullptr
                ));
            }
        }
        break;
    default:
        {
            RIO_GL_CALL(glTexImage2D(
                GL_TEXTURE_2D,
                0,
                nativeFormat.internalformat,
                width,
                height,
                0,
                nativeFormat.format,
                nativeFormat.type,
                image
            ));

            if (mipLevels > 1)
            {
                for (u32 i = 0; i < mipLevels - 2; i++)
                    RIO_GL_CALL(glTexImage2D(
                        GL_TEXTURE_2D,
                        i + 1,
                        nativeFormat.internalformat,
                        std::max(width  >> (i + 1), 1u),
                        std::max(height >> (i + 1), 1u),
                        0,
                        nativeFormat.format,
                        nativeFormat.type,
                        mipmaps ? ((u8*)mipmaps + mipLevelOffset[i]) : nullptr
                    ));

                RIO_GL_CALL(glTexImage2D(
                    GL_TEXTURE_2D,
                    (mipLevels - 2) + 1,
                    nativeFormat.internalformat,
                    std::max(width  >> ((mipLevels - 2) + 1), 1u),
                    std::max(height >> ((mipLevels - 2) + 1), 1u),
                    0,
                    nativeFormat.format,
                    nativeFormat.type,
                    mipmaps ? ((u8*)mipmaps + mipLevelOffset[(mipLevels - 2)]) : nullptr
                ));
            }
        }
        break;
    }
}

}

#endif // RIO_IS_WIN
