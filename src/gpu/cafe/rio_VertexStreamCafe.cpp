#include <misc/rio_Types.h>

#if RIO_IS_CAFE

#include <gpu/rio_VertexStream.h>

#include <gx2/enum.h>

namespace rio {

void VertexStream::setLayout(u8 location, Format format, u32 offset)
{
    mLocation = location;
    mFormat = format;
    mOffset = offset;

    switch (format)
    {
    case FORMAT_8_UNORM:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_UNORM_8;
        break;
    case FORMAT_8_8_UNORM:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_UNORM_8_8;
        break;
    case FORMAT_8_8_8_8_UNORM:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_UNORM_8_8_8_8;
        break;
    case FORMAT_8_UINT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_UINT_8;
        break;
    case FORMAT_8_8_UINT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_UINT_8_8;
        break;
    case FORMAT_8_8_8_8_UINT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_UINT_8_8_8_8;
        break;
    case FORMAT_8_UINT_TO_FLOAT:
        mInternalFormat = 0x800;
        break;
    case FORMAT_8_8_UINT_TO_FLOAT:
        mInternalFormat = 0x804;
        break;
    case FORMAT_8_8_8_8_UINT_TO_FLOAT:
        mInternalFormat = 0x80A;
        break;
    case FORMAT_8_SNORM:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_SNORM_8;
        break;
    case FORMAT_8_8_SNORM:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_SNORM_8_8;
        break;
    case FORMAT_8_8_8_8_SNORM:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_SNORM_8_8_8_8;
        break;
    case FORMAT_8_SINT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_SINT_8;
        break;
    case FORMAT_8_8_SINT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_SINT_8_8;
        break;
    case FORMAT_8_8_8_8_SINT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_SINT_8_8_8_8;
        break;
    case FORMAT_8_SINT_TO_FLOAT:
        mInternalFormat = 0xA00;
        break;
    case FORMAT_8_8_SINT_TO_FLOAT:
        mInternalFormat = 0xA04;
        break;
    case FORMAT_8_8_8_8_SINT_TO_FLOAT:
        mInternalFormat = 0xA0A;
        break;
    case FORMAT_10_11_11_FLOAT:
        mInternalFormat = 0x809;
        break;
    case FORMAT_10_10_10_2_UNORM:
        mInternalFormat = 0x00B;
        break;
    case FORMAT_10_10_10_2_UINT:
        mInternalFormat = 0x10B;
        break;
    case FORMAT_10_10_10_2_SNORM:
        mInternalFormat = 0x20B;
        break;
    case FORMAT_10_10_10_2_SINT:
        mInternalFormat = 0x30B;
        break;
    case FORMAT_16_UNORM:
        mInternalFormat = 0x002;
        break;
    case FORMAT_16_16_UNORM:
        mInternalFormat = 0x007;
        break;
    case FORMAT_16_16_16_16_UNORM:
        mInternalFormat = 0x00E;
        break;
    case FORMAT_16_UINT:
        mInternalFormat = 0x102;
        break;
    case FORMAT_16_16_UINT:
        mInternalFormat = 0x107;
        break;
    case FORMAT_16_16_16_16_UINT:
        mInternalFormat = 0x10E;
        break;
    case FORMAT_16_UINT_TO_FLOAT:
        mInternalFormat = 0x802;
        break;
    case FORMAT_16_16_UINT_TO_FLOAT:
        mInternalFormat = 0x807;
        break;
    case FORMAT_16_16_16_16_UINT_TO_FLOAT:
        mInternalFormat = 0x80E;
        break;
    case FORMAT_16_SNORM:
        mInternalFormat = 0x202;
        break;
    case FORMAT_16_16_SNORM:
        mInternalFormat = 0x207;
        break;
    case FORMAT_16_16_16_16_SNORM:
        mInternalFormat = 0x20E;
        break;
    case FORMAT_16_SINT:
        mInternalFormat = 0x302;
        break;
    case FORMAT_16_16_SINT:
        mInternalFormat = 0x307;
        break;
    case FORMAT_16_16_16_16_SINT:
        mInternalFormat = 0x30E;
        break;
    case FORMAT_16_SINT_TO_FLOAT:
        mInternalFormat = 0xA02;
        break;
    case FORMAT_16_16_SINT_TO_FLOAT:
        mInternalFormat = 0xA07;
        break;
    case FORMAT_16_16_16_16_SINT_TO_FLOAT:
        mInternalFormat = 0xA0E;
        break;
    case FORMAT_16_FLOAT:
        mInternalFormat = 0x803;
        break;
    case FORMAT_16_16_FLOAT:
        mInternalFormat = 0x808;
        break;
    case FORMAT_16_16_16_16_FLOAT:
        mInternalFormat = 0x80F;
        break;
    case FORMAT_32_UINT:
        mInternalFormat = 0x105;
        break;
    case FORMAT_32_32_UINT:
        mInternalFormat = 0x10C;
        break;
    case FORMAT_32_32_32_UINT:
        mInternalFormat = 0x110;
        break;
    case FORMAT_32_32_32_32_UINT:
        mInternalFormat = 0x112;
        break;
    case FORMAT_32_SINT:
        mInternalFormat = 0x305;
        break;
    case FORMAT_32_32_SINT:
        mInternalFormat = 0x30C;
        break;
    case FORMAT_32_32_32_SINT:
        mInternalFormat = 0x310;
        break;
    case FORMAT_32_32_32_32_SINT:
        mInternalFormat = 0x312;
        break;
    case FORMAT_32_FLOAT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_FLOAT_32;
        break;
    case FORMAT_32_32_FLOAT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_FLOAT_32_32;
        break;
    case FORMAT_32_32_32_FLOAT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_FLOAT_32_32_32;
        break;
    case FORMAT_32_32_32_32_FLOAT:
        mInternalFormat = (u32)GX2_ATTRIB_FORMAT_FLOAT_32_32_32_32;
        break;
    default:
        RIO_ASSERT(false);
    }
}

}

#endif // RIO_IS_CAFE
