#include <misc/rio_Types.h>

#if RIO_IS_WIN

#include <gfx/rio_Window.h>
#include <gfx/lyr/rio_Layer.h>
#include <gpu/rio_RenderState.h>
#include <gpu/rio_Shader.h>
#include <gpu/rio_VertexArray.h>

namespace {

static rio::Shader gScreenShader;
static rio::VertexArray* gVertexArray = nullptr;
static rio::VertexBuffer* gVertexBuffer = nullptr;

struct Vertex
{
    f32 pos[2];
    f32 texCoord[2];
};

static rio::VertexStream gPosStream     (0, rio::VertexStream::FORMAT_32_32_FLOAT, offsetof(Vertex, pos));
static rio::VertexStream gTexCoordStream(1, rio::VertexStream::FORMAT_32_32_FLOAT, offsetof(Vertex, texCoord));

static const Vertex vertices[] = {
    { { -1.0f,  1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f }, { 0.0f, 0.0f } },
    { {  1.0f, -1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f }, { 1.0f, 0.0f } },
    { {  1.0f,  1.0f }, { 1.0f, 1.0f } },
    { { -1.0f,  1.0f }, { 0.0f, 1.0f } }
};

}

namespace rio {

void Window::resizeCallback_(s32 width, s32 height)
{
    width  = std::max<s32>(1, width );
    height = std::max<s32>(1, height);

    mWidth = width;
    mHeight = height;

    // Set Color Buffer dimensions and format
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, mNativeWindow.mColorBufferTextureHandle));
    RIO_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, GL_NONE));

    // Set Depth-Stencil Buffer dimensions and format
    RIO_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, mNativeWindow.mDepthBufferHandle));
    RIO_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight));
    RIO_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE));

    // Set Depth-Stencil Buffer dimensions and format
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, mNativeWindow.mDepthBufferTextureHandle));
    RIO_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, GL_NONE));

    rio::lyr::Layer::onResize_(width, height);

    const auto& callback = mNativeWindow.mpOnResizeCallback;
    if (callback == nullptr)
        return;

    (*callback)(width, height);
}

void Window::resizeCallback_(GLFWwindow* glfw_window, s32 width, s32 height)
{
    rio::Window* const window = rio::Window::instance();
    if (window == nullptr || window->getNativeWindow().getGLFWwindow() != glfw_window)
        return;

    window->resizeCallback_(width, height);
}

bool Window::initialize_(bool resizable)
{
    // Initialize GLFW
    if (!glfwInit())
        return false;

    if (!resizable)
    {
        // Disable resizing
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    // Request OpenGL v3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Enforce double-buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // Create the window instance
    mNativeWindow.mpGLFWwindow = glfwCreateWindow(mWidth, mHeight, "Game", nullptr, nullptr);
    if (!mNativeWindow.mpGLFWwindow)
    {
        terminate_();
        return false;
    }

    // Query the Frame Buffer size
    glfwGetFramebufferSize(mNativeWindow.mpGLFWwindow,
                           reinterpret_cast<int*>(&mWidth),
                           reinterpret_cast<int*>(&mHeight));

    // Make context of window current
    glfwMakeContextCurrent(mNativeWindow.mpGLFWwindow);

    [[maybe_unused]] const char* renderer_str;
    RIO_GL_CALL(renderer_str = (const char*)glGetString(GL_RENDERER));
    RIO_LOG("Renderer: %s\n", renderer_str);

    // Set swap interval to 1 by default
    setSwapInterval(1);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        terminate_();
        return false;
    }

    // Check clip control extension
    if (!GLEW_VERSION_4_5 && !GLEW_ARB_clip_control)
    {
        terminate_();
        return false;
    }

    // Change coordinate-system to be compliant with GX2
    RIO_GL_CALL(glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE));

    // The screen will now be rendered upside-down.
    // Therefore, we will render it to our own frame buffer, then render that
    // frame buffer upside-down to the window frame buffer.

    // Load screen shader
    gScreenShader.load("screen_shader_win");

    // Create screen Vertex Array
    gVertexArray = new VertexArray();
    if (!gVertexArray)
    {
        terminate_();
        return false;
    }

    // Create screen Vertex Buffer
    gVertexBuffer = new VertexBuffer(vertices, sizeof(vertices), sizeof(Vertex), 0);
    if (!gVertexBuffer)
    {
        terminate_();
        return false;
    }

    // Process Vertex Array
    gVertexArray->addAttribute(gPosStream,      *gVertexBuffer);
    gVertexArray->addAttribute(gTexCoordStream, *gVertexBuffer);
    gVertexArray->process();

    // Create Frame Buffer
    if (!createFb_())
    {
        terminate_();
        return false;
    }

    // Bind our Frame Buffer
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mNativeWindow.mFramebufferHandle));
    RIO_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNativeWindow.mColorBufferTextureHandle, 0));
    RIO_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mNativeWindow.mDepthBufferHandle));

    // Enable scissor test
    RIO_GL_CALL(glEnable(GL_SCISSOR_TEST));

    if (resizable)
        glfwSetFramebufferSizeCallback(mNativeWindow.mpGLFWwindow, &Window::resizeCallback_);

    return true;
}

bool Window::isRunning() const
{
    return !glfwWindowShouldClose(mNativeWindow.mpGLFWwindow);
}

void Window::terminate_()
{
    destroyFb_();

    if (gVertexBuffer)
    {
        delete gVertexBuffer;
        gVertexBuffer = nullptr;
    }

    if (gVertexArray)
    {
        delete gVertexArray;
        gVertexArray = nullptr;
    }

    gScreenShader.unload();

    glfwTerminate();
}

bool Window::createFb_()
{
    // Generate OpenGL Frame Buffer
    RIO_GL_CALL(glGenFramebuffers(1, &mNativeWindow.mFramebufferHandle));
    if (mNativeWindow.mFramebufferHandle == GL_NONE)
        return false;

    // Bind it
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mNativeWindow.mFramebufferHandle));

    // Generate Color Buffer as OpenGL texture
    RIO_GL_CALL(glGenTextures(1, &mNativeWindow.mColorBufferTextureHandle));
    if (mNativeWindow.mColorBufferTextureHandle == GL_NONE)
        return false;

    // Set Color Buffer dimensions and format
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, mNativeWindow.mColorBufferTextureHandle));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));
    RIO_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, GL_NONE));
    mNativeWindow.mColorBufferTextureFormat = TEXTURE_FORMAT_R8_G8_B8_A8_UNORM;

    // Attach it to the Frame Buffer
    RIO_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNativeWindow.mColorBufferTextureHandle, 0));

    // Generate Depth-Stencil Buffer as OpenGL render target
    RIO_GL_CALL(glGenRenderbuffers(1, &mNativeWindow.mDepthBufferHandle));
    if (mNativeWindow.mDepthBufferHandle == GL_NONE)
        return false;

    // Set Depth-Stencil Buffer dimensions and format
    RIO_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, mNativeWindow.mDepthBufferHandle));
    RIO_GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight));
    RIO_GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE));

    // Attach it to the Frame Buffer
    RIO_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mNativeWindow.mDepthBufferHandle));

    // Check Frame Buffer completeness
    GLenum framebuffer_status;
    RIO_GL_CALL(framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE)
        return false;

    // Generate Depth-Stencil Buffer copy texture
    RIO_GL_CALL(glGenTextures(1, &mNativeWindow.mDepthBufferTextureHandle));
    if (mNativeWindow.mDepthBufferTextureHandle == GL_NONE)
        return false;

    // Set Depth-Stencil Buffer dimensions and format
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, mNativeWindow.mDepthBufferTextureHandle));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));
    RIO_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    RIO_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, GL_NONE));
    mNativeWindow.mDepthBufferTextureFormat = TextureFormat(0x00000011);

    // Create the source framebuffer with a depth-stencil renderbuffer
    RIO_GL_CALL(glGenFramebuffers(1, &mNativeWindow.mDepthBufferCopyFramebufferSrc));
    if (mNativeWindow.mDepthBufferCopyFramebufferSrc == GL_NONE)
        return false;

    // Bind it
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mNativeWindow.mDepthBufferCopyFramebufferSrc));

    // Attach the depth-stencil renderbuffer
    RIO_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mNativeWindow.mDepthBufferHandle));

    // Check Frame Buffer completeness
    RIO_GL_CALL(framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE)
        return false;

    // Create the destination framebuffer with a depth-stencil texture
    RIO_GL_CALL(glGenFramebuffers(1, &mNativeWindow.mDepthBufferCopyFramebufferDst));
    if (mNativeWindow.mDepthBufferCopyFramebufferDst == GL_NONE)
        return false;

    // Bind it
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mNativeWindow.mDepthBufferCopyFramebufferDst));

    // Attach the depth-stencil texture
    RIO_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mNativeWindow.mDepthBufferTextureHandle, 0));

    // Check Frame Buffer completeness
    RIO_GL_CALL(framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (framebuffer_status != GL_FRAMEBUFFER_COMPLETE)
        return false;

    return true;
}

void Window::destroyFb_()
{
    if (mNativeWindow.mDepthBufferCopyFramebufferDst != GL_NONE)
    {
        RIO_GL_CALL(glDeleteFramebuffers(1, &mNativeWindow.mDepthBufferCopyFramebufferDst));
        mNativeWindow.mDepthBufferCopyFramebufferDst = GL_NONE;
    }

    if (mNativeWindow.mDepthBufferCopyFramebufferSrc != GL_NONE)
    {
        RIO_GL_CALL(glDeleteFramebuffers(1, &mNativeWindow.mDepthBufferCopyFramebufferSrc));
        mNativeWindow.mDepthBufferCopyFramebufferSrc = GL_NONE;
    }

    if (mNativeWindow.mDepthBufferTextureHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteTextures(1, &mNativeWindow.mDepthBufferTextureHandle));
        mNativeWindow.mDepthBufferTextureHandle = GL_NONE;
    }

    if (mNativeWindow.mDepthBufferHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteRenderbuffers(1, &mNativeWindow.mDepthBufferHandle));
        mNativeWindow.mDepthBufferHandle = GL_NONE;
    }

    if (mNativeWindow.mColorBufferTextureHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteTextures(1, &mNativeWindow.mColorBufferTextureHandle));
        mNativeWindow.mColorBufferTextureHandle = GL_NONE;
    }

    if (mNativeWindow.mFramebufferHandle != GL_NONE)
    {
        RIO_GL_CALL(glDeleteFramebuffers(1, &mNativeWindow.mFramebufferHandle));
        mNativeWindow.mFramebufferHandle = GL_NONE;
    }
}

void Window::makeContextCurrent() const
{
    glfwMakeContextCurrent(mNativeWindow.mpGLFWwindow);

    // Bind our Frame Buffer
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mNativeWindow.mFramebufferHandle));
    RIO_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNativeWindow.mColorBufferTextureHandle, 0));
    RIO_GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mNativeWindow.mDepthBufferHandle));
}

void Window::setSwapInterval(u32 swap_interval)
{
    glfwSwapInterval(swap_interval);
}

void Window::setVpToFb_() const
{
    RIO_GL_CALL(glViewport(0, 0, mWidth, mHeight));
    RIO_GL_CALL(glDepthRange(0.f, 1.f));
    RIO_GL_CALL(glScissor(0, 0, mWidth, mHeight));
}

void Window::restoreVp_() const
{
    RIO_GL_CALL(glViewport(Graphics::sViewportX, Graphics::sViewportY, Graphics::sViewportWidth, Graphics::sViewportHeight));
    RIO_GL_CALL(glDepthRange(Graphics::sViewportNear, Graphics::sViewportFar));
    RIO_GL_CALL(glScissor(Graphics::sScissorX, Graphics::sScissorY, Graphics::sScissorWidth, Graphics::sScissorHeight));
}

void Window::swapBuffers() const
{
    // Bind the default (window) Frame Buffer
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE));

    // Set viewport and scissor
    setVpToFb_();

    // Disable Depth test
    rio::RenderState render_state;
    render_state.setDepthTestEnable(false);
    render_state.apply();

    // Bind the screen shader and vertex array
    gScreenShader.bind();
    gVertexArray->bind();

    // Bind screen texture (Color Buffer texture)
    RIO_GL_CALL(glActiveTexture(GL_TEXTURE0));
    RIO_GL_CALL(glBindTexture(GL_TEXTURE_2D, mNativeWindow.mColorBufferTextureHandle));

    // Draw it
    RIO_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

    // Swap front and back buffers
    glfwSwapBuffers(mNativeWindow.mpGLFWwindow);
    // Poll for and process events
    glfwPollEvents();

    // Restore our Frame Buffer
    RIO_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, mNativeWindow.mFramebufferHandle));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearColor(f32 r, f32 g, f32 b, f32 a)
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear using the given color
    RIO_GL_CALL(glClearColor(r, g, b, a));
    RIO_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearDepth()
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear
    RIO_GL_CALL(glDepthMask(GL_TRUE));
    RIO_GL_CALL(glClearDepth(1.0f));
    RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearDepth(f32 depth)
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear
    RIO_GL_CALL(glDepthMask(GL_TRUE));
    RIO_GL_CALL(glClearDepth(depth));
    RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearStencil()
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear
    RIO_GL_CALL(glClearStencil(0));
    RIO_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearStencil(u8 stencil)
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear
    RIO_GL_CALL(glClearStencil(stencil));
    RIO_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearDepthStencil()
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear
    RIO_GL_CALL(glDepthMask(GL_TRUE));
    RIO_GL_CALL(glClearDepth(1.0f));
    RIO_GL_CALL(glClearStencil(0));
    RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::clearDepthStencil(f32 depth, u8 stencil)
{
    // Make sure window context is current
    makeContextCurrent();

    // Set viewport and scissor
    setVpToFb_();

    // Clear
    RIO_GL_CALL(glDepthMask(GL_TRUE));
    RIO_GL_CALL(glClearDepth(depth));
    RIO_GL_CALL(glClearStencil(stencil));
    RIO_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    // Restore viewport and scissor
    restoreVp_();
}

void Window::updateDepthBufferTexture_()
{
    rio::Graphics::setViewport(0, 0, mWidth, mHeight);
    rio::Graphics::setScissor(0, 0, mWidth, mHeight);

    // Blit the depth-stencil renderbuffer to the depth-stencil texture
    RIO_GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, mNativeWindow.mDepthBufferCopyFramebufferSrc));
    RIO_GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mNativeWindow.mDepthBufferCopyFramebufferDst));
    RIO_GL_CALL(glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST));
}

}

#endif // RIO_IS_WIN
