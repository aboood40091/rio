#include <filedevice/rio_FileDeviceMgr.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_RenderState.h>

namespace rio {

Window* Window::sInstance = nullptr;

bool Window::createSingleton(u32 width, u32 height)
{
    if (sInstance)
        return false;

    if (!FileDeviceMgr::instance())
        return false;

    Window* window = new Window(width, height);
    if (!window->initialize_())
    {
        delete window;
        return false;
    }

    sInstance = window;

    Graphics::setViewport(0, 0, window->getWidth(), window->getHeight());
    Graphics::setScissor(0, 0, window->getWidth(), window->getHeight());

    window->clearColor(0.0f, 0.0f, 0.0f);
    window->clearDepthStencil();

    RenderState def_render_state;
    def_render_state.apply();

    return true;
}

void Window::destroySingleton()
{
    if (!sInstance)
        return;

    sInstance->terminate_();
    delete sInstance;
    sInstance = nullptr;
}

}
