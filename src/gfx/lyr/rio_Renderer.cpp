#include <gfx/rio_Graphics.h>
#include <gfx/rio_Window.h>
#include <gfx/lyr/rio_Renderer.h>

namespace rio { namespace lyr {

Renderer* Renderer::sInstance = nullptr;

bool Renderer::createSingleton()
{
    if (sInstance)
        return false;

    sInstance = new Renderer();
    return true;
}

void Renderer::destroySingleton()
{
    if (!sInstance)
        return;

    delete sInstance;
    sInstance = nullptr;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    clearLayers();
}

void Renderer::render() const
{
    Window* const p_window = Window::instance();

    Graphics::setViewport(0, 0, p_window->getWidth(), p_window->getHeight());
    Graphics::setScissor(0, 0, p_window->getWidth(), p_window->getHeight());

    bool viewport_changed = false;
    bool scissor_changed = false;

    for (const Layer* p_layer : mLayers)
    {
        const Layer& layer = *p_layer;

        if (layer.mFlags.isOn(Layer::FLAGS_CLEAR_COLOR_BUFFER))
            p_window->clearColor(layer.mClearColor.r, layer.mClearColor.g, layer.mClearColor.b, layer.mClearColor.a);

        if (layer.mFlags.isOn(Layer::FLAGS_CLEAR_DEPTH_STENCIL_BUFFER))
            p_window->clearDepthStencil(layer.mClearDepth, layer.mClearDepth);

        else if (layer.mFlags.isOn(Layer::FLAGS_CLEAR_DEPTH_BUFFER))
            p_window->clearDepth(layer.mClearDepth);

        else if (layer.mFlags.isOn(Layer::FLAGS_CLEAR_STENCIL_BUFFER))
            p_window->clearStencil(layer.mClearDepth);

        if (layer.mFlags.isOn(Layer::FLAGS_SET_VIEWPORT))
        {
            Graphics::setViewport(layer.mViewport.x, layer.mViewport.y, layer.mViewport.width, layer.mViewport.height, layer.mViewport.near, layer.mViewport.far, layer.mViewport.frame_buffer_height);
            viewport_changed = true;
        }
        else if (viewport_changed)
        {
            Graphics::setViewport(0, 0, p_window->getWidth(), p_window->getHeight());
        }

        if (layer.mFlags.isOn(Layer::FLAGS_SET_SCISSOR))
        {
            Graphics::setScissor(layer.mScissor.x, layer.mScissor.y, layer.mScissor.width, layer.mScissor.height, layer.mScissor.frame_buffer_height);
            scissor_changed = true;
        }
        else if (scissor_changed)
        {
            Graphics::setScissor(0, 0, p_window->getWidth(), p_window->getHeight());
        }

        u32 render_step_idx = 0;

        for (const RenderStep& render_step : layer.mRenderSteps)
        {
            for (const DrawMethod& draw_method : render_step.mDrawMethods)
                (draw_method.mObjPtr->*(draw_method.mFuncPtr))({ layer, render_step_idx });

            render_step_idx++;
        }
    }
}

} }
