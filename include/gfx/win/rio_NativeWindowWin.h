#ifndef RIO_GFX_NATIVE_WINDOW_WIN_H
#define RIO_GFX_NATIVE_WINDOW_WIN_H

#include <misc/gl/rio_GL.h>

namespace rio {

class Window;

class NativeWindow
{
public:
    NativeWindow()
        : mpGLFWwindow(nullptr)
        , mFramebufferHandle(GL_NONE)
        , mColorBufferTextureHandle(GL_NONE)
        , mColorBufferTextureFormat(TEXTURE_FORMAT_INVALID)
        , mDepthBufferHandle(GL_NONE)
        , mDepthBufferTextureHandle(GL_NONE)
        , mDepthBufferTextureFormat(TEXTURE_FORMAT_INVALID)
        , mDepthBufferCopyFramebufferSrc(GL_NONE)
        , mDepthBufferCopyFramebufferDst(GL_NONE)
    {
    }

    GLFWwindow* getGLFWwindow() const { return mpGLFWwindow; }

    GLuint getFramebufferHandle() const { return mFramebufferHandle; }

    GLuint getColorBufferTextureHandle() const { return mColorBufferTextureHandle; }
    TextureFormat getColorBufferTextureFormat() const { return mColorBufferTextureFormat; }

    GLuint getDepthBufferHandle() const { return mDepthBufferHandle; }

    GLuint getDepthBufferTextureHandle() const { return mDepthBufferTextureHandle; }
    TextureFormat getDepthBufferTextureFormat() const { return mDepthBufferTextureFormat; }


private:
    GLFWwindow* mpGLFWwindow;

    GLuint mFramebufferHandle;

    GLuint mColorBufferTextureHandle;
    TextureFormat mColorBufferTextureFormat;

    GLuint mDepthBufferHandle;

    GLuint mDepthBufferTextureHandle;
    TextureFormat mDepthBufferTextureFormat;

    GLuint mDepthBufferCopyFramebufferSrc;
    GLuint mDepthBufferCopyFramebufferDst;

    friend class Window;
};

}

#endif // RIO_GFX_NATIVE_WINDOW_WIN_H
