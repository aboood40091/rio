#ifndef RIO_GFX_NATIVE_WINDOW_WIN_H
#define RIO_GFX_NATIVE_WINDOW_WIN_H

#include <misc/gl/rio_GL.h>

#include <thread>

namespace rio {

class Window;

class NativeWindow
{
public:
    typedef void (*OnResizeCallback)(s32 width, s32 height);

private:
    using Clock = std::chrono::steady_clock;
    using Duration = Clock::duration;
    using DurationF64 = std::chrono::duration<f64>;
    using TimePoint = std::chrono::time_point<Clock>;

public:
    NativeWindow()
        : mpGLFWwindow(nullptr)
        , mpOnResizeCallback(nullptr)
        , mFramebufferHandle(GL_NONE)
        , mColorBufferTextureHandle(GL_NONE)
        , mColorBufferTextureFormat(TEXTURE_FORMAT_INVALID)
        , mDepthBufferHandle(GL_NONE)
        , mDepthBufferTextureHandle(GL_NONE)
        , mDepthBufferTextureFormat(TEXTURE_FORMAT_INVALID)
        , mDepthBufferCopyFramebufferSrc(GL_NONE)
        , mDepthBufferCopyFramebufferDst(GL_NONE)
    {
        setSwapInterval_(1);
    }

    GLFWwindow* getGLFWwindow() const { return mpGLFWwindow; }

    GLuint getFramebufferHandle() const { return mFramebufferHandle; }

    GLuint getColorBufferTextureHandle() const { return mColorBufferTextureHandle; }
    TextureFormat getColorBufferTextureFormat() const { return mColorBufferTextureFormat; }

    GLuint getDepthBufferHandle() const { return mDepthBufferHandle; }

    GLuint getDepthBufferTextureHandle() const { return mDepthBufferTextureHandle; }
    TextureFormat getDepthBufferTextureFormat() const { return mDepthBufferTextureFormat; }

private:
    void setSwapInterval_(f64 swap_interval)
    {
        mFrameDuration = std::chrono::round<Duration>(DurationF64 { swap_interval / 60 });
        mFrameEndTarget = Clock::now() + mFrameDuration;
    }

    void onSwapBuffers_() const
    {
        const auto& now = Clock::now();
        if (mFrameEndTarget < now)
            mFrameEndTarget = now;
        else
        {
#if 0
            // This results in smooth FPS, but with very high CPU usage...
            while (mFrameEndTarget > Clock::now())
                ;
#else
            // This results in slightly choppy FPS, but with almost no impact on CPU usage.
            std::this_thread::sleep_until(mFrameEndTarget);
#endif
        }
        mFrameEndTarget += mFrameDuration;
    }

private:
    GLFWwindow* mpGLFWwindow;
    OnResizeCallback mpOnResizeCallback;

    GLuint mFramebufferHandle;

    GLuint mColorBufferTextureHandle;
    TextureFormat mColorBufferTextureFormat;

    GLuint mDepthBufferHandle;

    GLuint mDepthBufferTextureHandle;
    TextureFormat mDepthBufferTextureFormat;

    GLuint mDepthBufferCopyFramebufferSrc;
    GLuint mDepthBufferCopyFramebufferDst;

    Duration mFrameDuration;
    mutable TimePoint mFrameEndTarget;

    friend class Window;
};

}

#endif // RIO_GFX_NATIVE_WINDOW_WIN_H
