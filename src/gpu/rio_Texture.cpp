#include <gpu/rio_Texture.h>
#include <misc/rio_MemUtil.h>

namespace rio {

u8 TextureFormatUtil::getPixelByteSize(TextureFormat format)
{
    switch (format)
    {
    case TEXTURE_FORMAT_R8_UNORM:
    case TEXTURE_FORMAT_R8_UINT:
    case TEXTURE_FORMAT_R8_SNORM:
    case TEXTURE_FORMAT_R8_SINT:
  //case TEXTURE_FORMAT_R4_G4_UNORM:
        return 1;
    case DEPTH_TEXTURE_FORMAT_R16_UNORM:
    case TEXTURE_FORMAT_R8_G8_UNORM:
    case TEXTURE_FORMAT_R8_G8_UINT:
    case TEXTURE_FORMAT_R8_G8_SNORM:
    case TEXTURE_FORMAT_R8_G8_SINT:
    case TEXTURE_FORMAT_R5_G6_B5_UNORM:
    case TEXTURE_FORMAT_R5_G5_B5_A1_UNORM:
    case TEXTURE_FORMAT_R4_G4_B4_A4_UNORM:
        return 2;
    case TEXTURE_FORMAT_R32_UINT:
    case DEPTH_TEXTURE_FORMAT_R32_FLOAT:
    case TEXTURE_FORMAT_R8_G8_B8_A8_UNORM:
    case TEXTURE_FORMAT_R8_G8_B8_A8_UINT:
    case TEXTURE_FORMAT_R8_G8_B8_A8_SNORM:
    case TEXTURE_FORMAT_R8_G8_B8_A8_SINT:
    case TEXTURE_FORMAT_R8_G8_B8_A8_SRGB:
    case TEXTURE_FORMAT_R10_G10_B10_A2_UNORM:
    case TEXTURE_FORMAT_R10_G10_B10_A2_UINT:
  //case TEXTURE_FORMAT_R10_G10_B10_A2_SNORM:
  //case TEXTURE_FORMAT_R10_G10_B10_A2_SINT:
        return 4;
    case TEXTURE_FORMAT_BC1_UNORM:
    case TEXTURE_FORMAT_BC1_SRGB:
    case TEXTURE_FORMAT_BC4_UNORM:
    case TEXTURE_FORMAT_BC4_SNORM:
        return 8;
    case TEXTURE_FORMAT_BC2_UNORM:
    case TEXTURE_FORMAT_BC2_SRGB:
    case TEXTURE_FORMAT_BC3_UNORM:
    case TEXTURE_FORMAT_BC3_SRGB:
    case TEXTURE_FORMAT_BC5_UNORM:
    case TEXTURE_FORMAT_BC5_SNORM:
        return 16;
    default:
        RIO_ASSERT(false);
        return 0;
    }
}

bool TextureFormatUtil::isCompressed(TextureFormat format)
{
    switch (format)
    {
    case TEXTURE_FORMAT_BC1_UNORM:
    case TEXTURE_FORMAT_BC1_SRGB:
    case TEXTURE_FORMAT_BC2_UNORM:
    case TEXTURE_FORMAT_BC2_SRGB:
    case TEXTURE_FORMAT_BC3_UNORM:
    case TEXTURE_FORMAT_BC3_SRGB:
    case TEXTURE_FORMAT_BC4_UNORM:
    case TEXTURE_FORMAT_BC4_SNORM:
    case TEXTURE_FORMAT_BC5_UNORM:
    case TEXTURE_FORMAT_BC5_SNORM:
        return true;
    default:
        return false;
    }
}

bool TextureFormatUtil::isUsableAsRenderTargetColor(TextureFormat format)
{
    switch (format)
    {
    case TEXTURE_FORMAT_R8_UNORM:
    case TEXTURE_FORMAT_R8_UINT:
    case TEXTURE_FORMAT_R8_SNORM:
    case TEXTURE_FORMAT_R8_SINT:
#if RIO_IS_CAFE
    case DEPTH_TEXTURE_FORMAT_R16_UNORM:
#endif // RIO_IS_CAFE
    case TEXTURE_FORMAT_R8_G8_UNORM:
    case TEXTURE_FORMAT_R8_G8_UINT:
    case TEXTURE_FORMAT_R8_G8_SNORM:
    case TEXTURE_FORMAT_R8_G8_SINT:
    case TEXTURE_FORMAT_R5_G6_B5_UNORM:
    case TEXTURE_FORMAT_R5_G5_B5_A1_UNORM:
    case TEXTURE_FORMAT_R4_G4_B4_A4_UNORM:
    case TEXTURE_FORMAT_R32_UINT:
#if RIO_IS_CAFE
    case DEPTH_TEXTURE_FORMAT_R32_FLOAT:
#endif // RIO_IS_CAFE
    case TEXTURE_FORMAT_R8_G8_B8_A8_UNORM:
    case TEXTURE_FORMAT_R8_G8_B8_A8_UINT:
    case TEXTURE_FORMAT_R8_G8_B8_A8_SNORM:
    case TEXTURE_FORMAT_R8_G8_B8_A8_SINT:
    case TEXTURE_FORMAT_R8_G8_B8_A8_SRGB:
    case TEXTURE_FORMAT_R10_G10_B10_A2_UNORM:
    case TEXTURE_FORMAT_R10_G10_B10_A2_UINT:
  //case TEXTURE_FORMAT_R10_G10_B10_A2_SINT:
        return true;
    default:
        return false;
    }
}

bool TextureFormatUtil::isUsableAsRenderTargetDepth(TextureFormat format)
{
    switch (format)
    {
    case DEPTH_TEXTURE_FORMAT_R16_UNORM:
    case DEPTH_TEXTURE_FORMAT_R32_FLOAT:
    case DEPTH_FORMAT_D24_S8_UNORM:
#if RIO_IS_CAFE
    case DEPTH_FORMAT_D24_S8_FLOAT:
#endif // RIO_IS_CAFE
    case DEPTH_FORMAT_D32_FLOAT_S8_UINT_X24:
        return true;
    default:
        return false;
    }
}

bool TextureFormatUtil::hasStencil(TextureFormat format)
{
    switch (format)
    {
    case DEPTH_FORMAT_D24_S8_UNORM:
#if RIO_IS_CAFE
    case DEPTH_FORMAT_D24_S8_FLOAT:
#endif // RIO_IS_CAFE
    case DEPTH_FORMAT_D32_FLOAT_S8_UINT_X24:
        return true;
    default:
        return false;
    }
}

u32 TextureFormatUtil::getDefaultCompMap(TextureFormat format)
{
    static const u32 compMap[54] = {
        0x04040405, 0x00040405, 0x00010405, 0x04040405,
        0x04040405, 0x00040405, 0x00040405, 0x00010405,
        0x00010205, 0x00010205, 0x00010203, 0x00010203,
        0x03020100, 0x00040405, 0x00040405, 0x00010405,
        0x00010405, 0x04040405, 0x04040405, 0x04040405,
        0x04040405, 0x04040405, 0x00010205, 0x04040405,
        0x04040405, 0x00010203, 0x00010203, 0x03020100,
        0x00010405, 0x00010405, 0x00010405, 0x00010203,
        0x00010203, 0x04040405, 0x00010203, 0x00010203,
        0x04040405, 0x04040405, 0x04040405, 0x00010205,
        0x00010205, 0x00040405, 0x00010405, 0x00010205,
        0x04040405, 0x04040405, 0x04040405, 0x00010205,
        0x00010205, 0x00010203, 0x00010203, 0x00010203,
        0x00040405, 0x00010405
    };
    return compMap[format & 0x3f];
}

#if RIO_IS_WIN

bool TextureFormatUtil::getNativeTextureFormat(
    NativeTextureFormat& nativeFormat,
    TextureFormat format
)
{
    switch (format)
    {
    case TEXTURE_FORMAT_R8_UNORM:
        nativeFormat.internalformat = GL_R8;
        nativeFormat.format = GL_RED;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_UINT:
        nativeFormat.internalformat = GL_R8UI;
        nativeFormat.format = GL_RED_INTEGER;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_SNORM:
        nativeFormat.internalformat = GL_R8_SNORM;
        nativeFormat.format = GL_RED;
        nativeFormat.type = GL_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_SINT:
        nativeFormat.internalformat = GL_R8I;
        nativeFormat.format = GL_RED_INTEGER;
        nativeFormat.type = GL_BYTE;
        return true;
    case DEPTH_TEXTURE_FORMAT_R16_UNORM:
        nativeFormat.internalformat = GL_DEPTH_COMPONENT16;
        nativeFormat.format = GL_DEPTH_COMPONENT;
        nativeFormat.type = GL_UNSIGNED_SHORT;
        return true;
    case TEXTURE_FORMAT_R8_G8_UNORM:
        nativeFormat.internalformat = GL_RG8;
        nativeFormat.format = GL_RG;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_UINT:
        nativeFormat.internalformat = GL_RG8UI;
        nativeFormat.format = GL_RG_INTEGER;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_SNORM:
        nativeFormat.internalformat = GL_RG8_SNORM;
        nativeFormat.format = GL_RG;
        nativeFormat.type = GL_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_SINT:
        nativeFormat.internalformat = GL_RG8I;
        nativeFormat.format = GL_RG_INTEGER;
        nativeFormat.type = GL_BYTE;
        return true;
    case TEXTURE_FORMAT_R5_G6_B5_UNORM:
        nativeFormat.internalformat = GL_RGB565;
        nativeFormat.format = GL_RGB;
        nativeFormat.type = GL_UNSIGNED_SHORT_5_6_5;
        return true;
    case TEXTURE_FORMAT_R5_G5_B5_A1_UNORM:
        nativeFormat.internalformat = GL_RGB5_A1;
        nativeFormat.format = GL_RGBA;
        nativeFormat.type = GL_UNSIGNED_SHORT_5_5_5_1;
        return true;
    case TEXTURE_FORMAT_R4_G4_B4_A4_UNORM:
        nativeFormat.internalformat = GL_RGBA4;
        nativeFormat.format = GL_RGBA;
        nativeFormat.type = GL_UNSIGNED_SHORT_4_4_4_4;
        return true;
    case TEXTURE_FORMAT_R32_UINT:
        nativeFormat.internalformat = GL_R32UI;
        nativeFormat.format = GL_RED_INTEGER;
        nativeFormat.type = GL_UNSIGNED_INT;
        return true;
    case DEPTH_TEXTURE_FORMAT_R32_FLOAT:
        nativeFormat.internalformat = GL_DEPTH_COMPONENT32F;
        nativeFormat.format = GL_DEPTH_COMPONENT;
        nativeFormat.type = GL_FLOAT;
        return true;
    case TEXTURE_FORMAT_R8_G8_B8_A8_UNORM:
        nativeFormat.internalformat = GL_RGBA8;
        nativeFormat.format = GL_RGBA;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_B8_A8_UINT:
        nativeFormat.internalformat = GL_RGBA8UI;
        nativeFormat.format = GL_RGBA_INTEGER;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_B8_A8_SNORM:
        nativeFormat.internalformat = GL_RGBA8_SNORM;
        nativeFormat.format = GL_RGBA;
        nativeFormat.type = GL_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_B8_A8_SINT:
        nativeFormat.internalformat = GL_RGBA8I;
        nativeFormat.format = GL_RGBA_INTEGER;
        nativeFormat.type = GL_BYTE;
        return true;
    case TEXTURE_FORMAT_R8_G8_B8_A8_SRGB:
        nativeFormat.internalformat = GL_SRGB8_ALPHA8;
        nativeFormat.format = GL_RGBA;
        nativeFormat.type = GL_UNSIGNED_BYTE;
        return true;
    case TEXTURE_FORMAT_R10_G10_B10_A2_UNORM:
        nativeFormat.internalformat = GL_RGB10_A2;
        nativeFormat.format = GL_RGBA;
        nativeFormat.type = GL_UNSIGNED_INT_2_10_10_10_REV;
        return true;
    case TEXTURE_FORMAT_R10_G10_B10_A2_UINT:
        nativeFormat.internalformat = GL_RGB10_A2UI;
        nativeFormat.format = GL_RGBA_INTEGER;
        nativeFormat.type = GL_UNSIGNED_INT_2_10_10_10_REV;
        return true;
    case TEXTURE_FORMAT_BC1_UNORM:
        nativeFormat.internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC1_SRGB:
        nativeFormat.internalformat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC2_UNORM:
        nativeFormat.internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC2_SRGB:
        nativeFormat.internalformat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC3_UNORM:
        nativeFormat.internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC3_SRGB:
        nativeFormat.internalformat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC4_UNORM:
        nativeFormat.internalformat = GL_COMPRESSED_RED_RGTC1;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC4_SNORM:
        nativeFormat.internalformat = GL_COMPRESSED_SIGNED_RED_RGTC1;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC5_UNORM:
        nativeFormat.internalformat = GL_COMPRESSED_RG_RGTC2;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case TEXTURE_FORMAT_BC5_SNORM:
        nativeFormat.internalformat = GL_COMPRESSED_SIGNED_RG_RGTC2;
        nativeFormat.format = 0;
        nativeFormat.type = 0;
        return true;
    case DEPTH_FORMAT_D24_S8_UNORM:
        nativeFormat.internalformat = GL_DEPTH24_STENCIL8;
        nativeFormat.format = GL_DEPTH_STENCIL;
        nativeFormat.type = GL_UNSIGNED_INT_24_8;
        return true;
    case DEPTH_FORMAT_D32_FLOAT_S8_UINT_X24:
        nativeFormat.internalformat = GL_DEPTH32F_STENCIL8;
        nativeFormat.format = GL_DEPTH_STENCIL;
        nativeFormat.type = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
        return true;
    default:
        RIO_ASSERT(false);
        MemUtil::set(&nativeFormat, 0, sizeof(NativeTextureFormat));
        return false;
    }
}

#endif // RIO_IS_WIN

}
