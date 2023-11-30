#ifndef RIO_GPU_TEXTURE_H
#define RIO_GPU_TEXTURE_H

#include <misc/rio_Types.h>

#if RIO_IS_CAFE
#include <gx2/texture.h>
#elif RIO_IS_WIN
#include <misc/gl/rio_GL.h>
#endif

namespace rio {

enum TextureFormat : u32
{
    // U = Unsigned
    // S = Signed
    // NORM = Normalized to float
    // INT = Integer (Keep this in mind: https://stackoverflow.com/a/39578132)

    TEXTURE_FORMAT_INVALID              = 0,

    /*               8 bits              */

    TEXTURE_FORMAT_R8_UNORM             = 0x001,
    TEXTURE_FORMAT_R8_UINT              = 0x101,
    TEXTURE_FORMAT_R8_SNORM             = 0x201,
    TEXTURE_FORMAT_R8_SINT              = 0x301,

    // Not supported by OpenGL
  //TEXTURE_FORMAT_R4_G4_UNORM          = 0x002,

    /*              16 bits              */

    DEPTH_TEXTURE_FORMAT_R16_UNORM      = 0x005,

    TEXTURE_FORMAT_R8_G8_UNORM          = 0x007,
    TEXTURE_FORMAT_R8_G8_UINT           = 0x107,
    TEXTURE_FORMAT_R8_G8_SNORM          = 0x207,
    TEXTURE_FORMAT_R8_G8_SINT           = 0x307,

    TEXTURE_FORMAT_R5_G6_B5_UNORM       = 0x008,

    TEXTURE_FORMAT_R5_G5_B5_A1_UNORM    = 0x00A,

    TEXTURE_FORMAT_R4_G4_B4_A4_UNORM    = 0x00B,

    /*              32 bits              */

    TEXTURE_FORMAT_R32_UINT             = 0x10D,
    DEPTH_TEXTURE_FORMAT_R32_FLOAT      = 0x80E,

    TEXTURE_FORMAT_R8_G8_B8_A8_UNORM    = 0x01A,
    TEXTURE_FORMAT_R8_G8_B8_A8_UINT     = 0x11A,
    TEXTURE_FORMAT_R8_G8_B8_A8_SNORM    = 0x21A,
    TEXTURE_FORMAT_R8_G8_B8_A8_SINT     = 0x31A,
    TEXTURE_FORMAT_R8_G8_B8_A8_SRGB     = 0x41A,

    TEXTURE_FORMAT_R10_G10_B10_A2_UNORM = 0x019,
    TEXTURE_FORMAT_R10_G10_B10_A2_UINT  = 0x119,
    // Not supported by OpenGL
  //TEXTURE_FORMAT_R10_G10_B10_A2_SNORM = 0x219,
  //TEXTURE_FORMAT_R10_G10_B10_A2_SINT  = 0x319,

    /*            compressed           */

    TEXTURE_FORMAT_BC1_UNORM            = 0x031,
    TEXTURE_FORMAT_BC1_SRGB             = 0x431,

    TEXTURE_FORMAT_BC2_UNORM            = 0x032,
    TEXTURE_FORMAT_BC2_SRGB             = 0x432,

    TEXTURE_FORMAT_BC3_UNORM            = 0x033,
    TEXTURE_FORMAT_BC3_SRGB             = 0x433,

    TEXTURE_FORMAT_BC4_UNORM            = 0x034,
    TEXTURE_FORMAT_BC4_SNORM            = 0x234,

    TEXTURE_FORMAT_BC5_UNORM            = 0x035,
    TEXTURE_FORMAT_BC5_SNORM            = 0x235,

    /*      depth-stencil formats      */

    DEPTH_FORMAT_D24_S8_UNORM           = 0x011,
    DEPTH_FORMAT_D24_S8_FLOAT           = 0x811,
    DEPTH_FORMAT_D32_FLOAT_S8_UINT_X24  = 0x81C
};

#if RIO_IS_CAFE

typedef GX2Surface  NativeSurface2D;
typedef GX2Texture  NativeTexture2D;
typedef const GX2Texture* NativeTexture2DHandle;

#define RIO_NATIVE_TEXTURE_2D_HANDLE_NULL nullptr

#elif RIO_IS_WIN

struct NativeTextureFormat
{
    s32 internalformat;
    u32 format;
    u32 type;
};

struct NativeSurface2D
{
    u32                 width;
    u32                 height;
    u32                 mipLevels;
    TextureFormat       format;
    NativeTextureFormat nativeFormat;
    u32                 imageSize;
    u32                 _imageOffset;
    u32                 mipmapSize;
    u32                 _mipmapsOffset;
    u32                 mipLevelOffset[13];

    // Unions for compatibility between 32-bit and 64-bit:
    // (In the hopes of future 64-bit support, that is...)
    union
    {
        u64     _imagePtr;
        void*   image;
    };
    union
    {
        u64     _mipmapsPtr;
        void*   mipmaps;
    };
};
static_assert(sizeof(NativeSurface2D) == 0x70, "rio::NativeSurface2D size mismatch");

struct NativeTexture2D
{
    NativeSurface2D surface;
    u32             compMap;

    // Footer for file storage
    struct
    {
        u32 magic;
        u32 version;
        u32 _reserved;
    } _footer;
};
static_assert(sizeof(NativeTexture2D) == 0x80, "rio::NativeTexture2D size mismatch");

typedef GLuint NativeTexture2DHandle;

#define RIO_NATIVE_TEXTURE_2D_HANDLE_NULL GL_NONE

#endif

class TextureFormatUtil
{
public:
    static u8 getPixelByteSize(TextureFormat format);
    static bool isCompressed(TextureFormat format);
    static bool isUsableAsRenderTargetColor(TextureFormat format);
    static bool isUsableAsRenderTargetDepth(TextureFormat format);
    static bool hasStencil(TextureFormat format);
    static u32 getDefaultCompMap(TextureFormat format);

#if RIO_IS_WIN
    static bool getNativeTextureFormat(
        NativeTextureFormat& nativeFormat,
        TextureFormat format
    );
#endif // RIO_IS_WIN
};

class TextureSampler2D;

class Texture2D
{
public:
    Texture2D(const char* base_fname);

    Texture2D(const u8* file, u32 file_size)
        : mSelfAllocated(true)
    {
        load_(file, file_size);
    }

    Texture2D(NativeTexture2D& native_texture)
        : mSelfAllocated(false)
    {
        mTextureInner = native_texture;
        createHandle_();
    }

    Texture2D(TextureFormat format, u32 width, u32 height, u32 numMips);

    ~Texture2D();

private:
    Texture2D(const Texture2D&);
    Texture2D& operator=(const Texture2D&);

public:
    u32 getWidth() const { return mTextureInner.surface.width; }
    u32 getHeight() const { return mTextureInner.surface.height; }
    u32 getNumMips() const { return mTextureInner.surface.mipLevels; }
    TextureFormat getTextureFormat() const { return (TextureFormat)mTextureInner.surface.format; }

    u32 getCompMap() const { return mTextureInner.compMap; }
    void setCompMap(u32 compMap);

    const NativeTexture2D& getNativeTexture() const { return mTextureInner; }
    NativeTexture2DHandle getNativeTextureHandle() const { return mHandle; }

private:
    void load_(const u8* file, u32 file_size);
    void createHandle_();

private:
    NativeTexture2D         mTextureInner;  // Native texture.
    NativeTexture2DHandle   mHandle;        // Native texture handle.
    bool                    mSelfAllocated; // Is native texture allocated by this instance?
};

}

#endif // RIO_GPU_TEXTURE_H
