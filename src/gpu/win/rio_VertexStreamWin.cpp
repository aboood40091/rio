#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_VertexStream.h>

#include <misc/gl/rio_GL.h>

namespace rio {

void VertexStream::setLayout(u8 location, Format format, u32 offset)
{
    mLocation = location;
    mFormat = format;
    mOffset = offset;

    switch (format)
    {
    case FORMAT_8_UNORM:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_8_UNORM:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_8_8_8_UNORM:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_UINT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_8_UINT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_8_8_8_UINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_UINT_TO_FLOAT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_8_UINT_TO_FLOAT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_8_8_8_UINT_TO_FLOAT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_BYTE;
        break;
    case FORMAT_8_SNORM:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_8_SNORM:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_8_8_8_SNORM:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_SINT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_8_SINT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_8_8_8_SINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_SINT_TO_FLOAT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_8_SINT_TO_FLOAT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_8_8_8_8_SINT_TO_FLOAT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_BYTE;
        break;
    case FORMAT_10_11_11_FLOAT:
        mInternalFormat.elem_count = 3;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_INT_10F_11F_11F_REV;
        break;
    case FORMAT_10_10_10_2_UNORM:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_INT_2_10_10_10_REV;
        break;
    case FORMAT_10_10_10_2_UINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_INT_2_10_10_10_REV;
        break;
    case FORMAT_10_10_10_2_SNORM:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_INT_2_10_10_10_REV;
        break;
    case FORMAT_10_10_10_2_SINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_INT_2_10_10_10_REV;
        break;
    case FORMAT_16_UNORM:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_16_UNORM:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_16_16_16_UNORM:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_UINT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_16_UINT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_16_16_16_UINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_UINT_TO_FLOAT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_16_UINT_TO_FLOAT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_16_16_16_UINT_TO_FLOAT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_UNSIGNED_SHORT;
        break;
    case FORMAT_16_SNORM:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_16_SNORM:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_16_16_16_SNORM:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = true;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_SINT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_16_SINT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_16_16_16_SINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_SINT_TO_FLOAT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_16_SINT_TO_FLOAT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_16_16_16_SINT_TO_FLOAT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_SHORT;
        break;
    case FORMAT_16_FLOAT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_HALF_FLOAT;
        break;
    case FORMAT_16_16_FLOAT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_HALF_FLOAT;
        break;
    case FORMAT_16_16_16_16_FLOAT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_HALF_FLOAT;
        break;
    case FORMAT_32_UINT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_INT;
        break;
    case FORMAT_32_32_UINT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_INT;
        break;
    case FORMAT_32_32_32_UINT:
        mInternalFormat.elem_count = 3;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_INT;
        break;
    case FORMAT_32_32_32_32_UINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_UNSIGNED_INT;
        break;
    case FORMAT_32_SINT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_INT;
        break;
    case FORMAT_32_32_SINT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_INT;
        break;
    case FORMAT_32_32_32_SINT:
        mInternalFormat.elem_count = 3;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_INT;
        break;
    case FORMAT_32_32_32_32_SINT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = true;
        mInternalFormat.type = GL_INT;
        break;
    case FORMAT_32_FLOAT:
        mInternalFormat.elem_count = 1;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_FLOAT;
        break;
    case FORMAT_32_32_FLOAT:
        mInternalFormat.elem_count = 2;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_FLOAT;
        break;
    case FORMAT_32_32_32_FLOAT:
        mInternalFormat.elem_count = 3;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_FLOAT;
        break;
    case FORMAT_32_32_32_32_FLOAT:
        mInternalFormat.elem_count = 4;
        mInternalFormat.normalized = false;
        mInternalFormat.integer = false;
        mInternalFormat.type = GL_FLOAT;
        break;
    default:
        RIO_ASSERT(false);
    }
}

}

#endif // RIO_IS_WIN
