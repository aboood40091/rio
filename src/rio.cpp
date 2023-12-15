#include <rio.h>

#include <audio/rio_AudioMgr.h>
#include <controller/rio_ControllerMgr.h>
#include <filedevice/rio_FileDeviceMgr.h>
#include <gfx/lyr/rio_Renderer.h>
#include <gfx/mdl/res/rio_ModelCacher.h>
#include <gfx/rio_PrimitiveRenderer.h>
#include <gfx/rio_Window.h>
#include <task/rio_TaskMgr.h>

#if RIO_IS_CAFE
#include <whb/crash.h>
#include <whb/log_cafe.h>
#include <whb/log_udp.h>
#endif // RIO_IS_CAFE

namespace rio {

ITask* sRootTask = nullptr;
const InitializeArg cDefaultInitializeArg;

bool Initialize(const InitializeArg& arg)
{
#if RIO_IS_CAFE && defined(RIO_DEBUG)
    WHBInitCrashHandler();
    WHBLogCafeInit();
    WHBLogUdpInit();
#endif

    // Create the file device manager
    if (!FileDeviceMgr::createSingleton())
        return false;

    // Create the window
    if (!Window::createSingleton(
        arg.window.width, arg.window.height
#if RIO_IS_WIN
        , arg.window.resizable
        , arg.window.gl_major
        , arg.window.gl_minor
#endif // RIO_IS_WIN
    ))
    {
        FileDeviceMgr::destroySingleton();
        return false;
    }

    // Create the task manager
    if (!TaskMgr::createSingleton())
    {
        Window::destroySingleton();
        FileDeviceMgr::destroySingleton();
        return false;
    }

    // Create the controller manager
    if (!ControllerMgr::createSingleton())
    {
        TaskMgr::destroySingleton();
        Window::destroySingleton();
        FileDeviceMgr::destroySingleton();
        return false;
    }

    // Create the primitive renderer
    if (!PrimitiveRenderer::createSingleton(arg.primitive_renderer.shader_path))
    {
        ControllerMgr::destroySingleton();
        TaskMgr::destroySingleton();
        Window::destroySingleton();
        FileDeviceMgr::destroySingleton();
        return false;
    }

    // Create the renderer instance
    if (!lyr::Renderer::createSingleton())
    {
        PrimitiveRenderer::destroySingleton();
        ControllerMgr::destroySingleton();
        TaskMgr::destroySingleton();
        Window::destroySingleton();
        FileDeviceMgr::destroySingleton();
        return false;
    }

    // Create the model cacher instance
    if (!mdl::res::ModelCacher::createSingleton())
    {
        lyr::Renderer::destroySingleton();
        PrimitiveRenderer::destroySingleton();
        ControllerMgr::destroySingleton();
        TaskMgr::destroySingleton();
        Window::destroySingleton();
        FileDeviceMgr::destroySingleton();
        return false;
    }

    // Create the audio manager instance
    if (!AudioMgr::createSingleton())
        RIO_LOG("rio::Initialize: Failed to create AudioMgr.\n");

    return true;
}

void EnterMainLoop()
{
    // Get window instance
    Window* window = Window::instance();

    // Main loop
    while (window->isRunning())
    {
        // Update the task manager
        TaskMgr::instance()->calc();

        // Render
        lyr::Renderer::instance()->render();

        // Swap the front and back buffers
        window->swapBuffers();
    }
}

void Exit()
{
    // Destroy the root task upon quitting
    if (sRootTask)
    {
        TaskMgr::instance()->destroyTask(sRootTask);
        sRootTask = nullptr;
    }

    // Destroy the audio manager upon quitting
    AudioMgr::destroySingleton();

    // Destroy the model cacher instance upon quitting
    mdl::res::ModelCacher::destroySingleton();

    // Destroy the renderer instance upon quitting
    lyr::Renderer::destroySingleton();

    // Destroy the primitive renderer upon quitting
    PrimitiveRenderer::destroySingleton();

    // Destroy the controller manager upon quitting
    ControllerMgr::destroySingleton();

    // Destroy the task manager upon quitting
    TaskMgr::destroySingleton();

    // Destroy the window upon quitting
    Window::destroySingleton();

    // Destroy the file device manager upon quitting
    FileDeviceMgr::destroySingleton();
}

} // namespace rio
