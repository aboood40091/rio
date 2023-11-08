#ifndef RIO_GPU_TEXTURE_2D_UTIL_WIN_H
#define RIO_GPU_TEXTURE_2D_UTIL_WIN_H

#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gpu/rio_Texture.h>

namespace rio {

class Texture2DUtil
{
public:
    static u32 calcImageSize(
        TextureFormat format,
        u32 width,
        u32 height
    );

    static u32 calcMipmapSize(
        TextureFormat format,
        u32 width,
        u32 height,
        u32 mipLevels,
        u32* mipLevelOffset = nullptr
    );

    static u32 createHandle();
    static void destroyHandle(u32 handle);

    static void bind(u32 handle);

    static void setSwizzleCurrent(u32 compMap);
    static void setSwizzle(u32 handle, u32 compMap)
    {
        bind(handle);
        setSwizzleCurrent(compMap);
    }

    static void setNumMipsCurrent(u32 mipLevels);
    static void setNumMips(u32 handle, u32 mipLevels)
    {
        bind(handle);
        setNumMipsCurrent(mipLevels);
    }

    static void uploadTextureCurrent(
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
    );

    static void uploadTexture(
        u32 handle,
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
        bind(handle);
        uploadTextureCurrent(
            format,
            nativeFormat,
            width,
            height,
            mipLevels,
            imageSize,
            image,
            mipmapSize,
            mipmaps,
            mipLevelOffset
        );
    }

    static void uploadTextureCurrent(
        TextureFormat format,
        const NativeTextureFormat& nativeFormat,
        u32 width,
        u32 height,
        u32 imageSize,
        const void* image
    )
    {
        uploadTextureCurrent(
            format,
            nativeFormat,
            width,
            height,
            1,
            imageSize,
            image,
            0,
            nullptr,
            nullptr
        );
    }

    static void uploadTexture(
        u32 handle,
        TextureFormat format,
        const NativeTextureFormat& nativeFormat,
        u32 width,
        u32 height,
        u32 imageSize,
        const void* image
    )
    {
        bind(handle);
        uploadTextureCurrent(
            format,
            nativeFormat,
            width,
            height,
            imageSize,
            image
        );
    }

    static u32 createHandle(
        TextureFormat format,
        const NativeTextureFormat& nativeFormat,
        u32 width,
        u32 height,
        u32 mipLevels,
        u32 imageSize,
        const void* image,
        u32 mipmapSize,
        const void* mipmaps,
        const u32* mipLevelOffset,
        u32 compMap
    )
    {
        u32 handle = createHandle();
        bind(handle);
        setSwizzleCurrent(compMap);
        setNumMipsCurrent(mipLevels);
        uploadTextureCurrent(
            format,
            nativeFormat,
            width,
            height,
            mipLevels,
            imageSize,
            image,
            mipmapSize,
            mipmaps,
            mipLevelOffset
        );
        return handle;
    }

    static u32 createHandle(
        TextureFormat format,
        const NativeTextureFormat& nativeFormat,
        u32 width,
        u32 height,
        u32 imageSize,
        const void* image,
        u32 compMap
    )
    {
        u32 handle = createHandle();
        bind(handle);
        setSwizzleCurrent(compMap);
        setNumMipsCurrent(1);
        uploadTextureCurrent(
            format,
            nativeFormat,
            width,
            height,
            imageSize,
            image
        );
        return handle;
    }
};

}

#endif // RIO_IS_WIN

#endif // RIO_GPU_TEXTURE_2D_UTIL_WIN_H
