#include <gfx/lyr/rio_Renderer.h>
#include <gfx/rio_Window.h>

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
}

Layer::iterator Renderer::addLayer(const char* name, s32 priority)
{
    return mLayers.emplace(name, priority);
}

void Renderer::removeLayer(Layer::iterator it)
{
    mLayers.erase(it);
}

void Renderer::clearLayers()
{
    mLayers.clear();
}

void Renderer::render() const
{
    Window* const window = Window::instance();

    for (const Layer& layer : mLayers)
    {
        if (layer.mClearFlags.isOn(Layer::FLAGS_CLEAR_COLOR_BUFFER))
            window->clearColor(layer.mClearColor.r, layer.mClearColor.g, layer.mClearColor.b, layer.mClearColor.a);

        if (layer.mClearFlags.isOn(Layer::FLAGS_CLEAR_DEPTH_STENCIL_BUFFER))
            window->clearDepthStencil(layer.mClearDepth, layer.mClearDepth);

        else if (layer.mClearFlags.isOn(Layer::FLAGS_CLEAR_DEPTH_BUFFER))
            window->clearDepth(layer.mClearDepth);

        else if (layer.mClearFlags.isOn(Layer::FLAGS_CLEAR_STENCIL_BUFFER))
            window->clearStencil(layer.mClearDepth);

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
