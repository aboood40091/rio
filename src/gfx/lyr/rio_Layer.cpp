#include <gfx/lyr/rio_Layer.h>
#include <gfx/rio_Camera.h>
#include <gfx/rio_Projection.h>
#include <gfx/rio_Window.h>
#include <math/rio_Matrix.h>

namespace {

class IdentityCamera : public rio::Camera
{
public:
    virtual void getMatrix(rio::BaseMtx34f* dst) const
    {
        RIO_ASSERT(dst);
        *dst = static_cast<const rio::BaseMtx34f&>(rio::Matrix34f::ident);
    }
};

}

namespace rio { namespace lyr {

Layer::Layer(const char* name, s32 priority)
    : mName(name)
    , mPriority(priority)
    , mClearColor{ Color4f::cBlack }
    , mClearDepth(1.0f)
    , mClearStencil(0)
    , mClearFlags(0)
{
    static IdentityCamera sDefaultCamera;

    Window* const window = Window::instance();
    RIO_ASSERT(window);

    const f32 window_width_2 = window->getWidth() * 0.5f;
    const f32 window_height_2 = window->getHeight() * 0.5f;

    static OrthoProjection sDefaultProjection(-1000.0f, 1000.0f, window_height_2, -window_height_2, -window_width_2, window_width_2);

    mpCamera = &sDefaultCamera;
    mpProjection = &sDefaultProjection;
}

Layer::~Layer()
{
}

bool Layer::operator<(const Layer& rhs) const
{
    return mPriority > rhs.mPriority;
}

void Layer::setCamera(const Camera* camera)
{
    RIO_ASSERT(camera);
    mpCamera = camera;
}

void Layer::setProjection(const Projection* projection)
{
    RIO_ASSERT(projection);
    mpProjection = projection;
}

void Layer::setClearColor(const Color4f& color)
{
    mClearColor = color;
    mClearFlags.set(FLAGS_CLEAR_COLOR_BUFFER);
}

void Layer::setClearDepth(f32 depth)
{
    mClearDepth = depth;
    mClearFlags.set(FLAGS_CLEAR_DEPTH_BUFFER);
}

void Layer::setClearStencil(u8 stencil)
{
    mClearStencil = stencil;
    mClearFlags.set(FLAGS_CLEAR_STENCIL_BUFFER);
}

void Layer::setClearDepthStencil(f32 depth, u8 stencil)
{
    mClearDepth = depth;
    mClearStencil = stencil;
    mClearFlags.set(FLAGS_CLEAR_DEPTH_STENCIL_BUFFER);
}

void Layer::resetClearColor()
{
    mClearFlags.reset(FLAGS_CLEAR_COLOR_BUFFER);
}

void Layer::resetClearDepth()
{
    mClearFlags.reset(FLAGS_CLEAR_DEPTH_BUFFER);
}

void Layer::resetClearStencil()
{
    mClearFlags.reset(FLAGS_CLEAR_STENCIL_BUFFER);
}

void Layer::resetClearDepthStencil()
{
    mClearFlags.reset(FLAGS_CLEAR_DEPTH_STENCIL_BUFFER);
}

void Layer::addRenderStep(const char* name)
{
    mRenderSteps.emplace_back(name);
}

void Layer::clearRenderSteps()
{
    mRenderSteps.clear();
}

void Layer::addDrawMethod(u32 render_step_idx, const DrawMethod& draw_method)
{
    if (render_step_idx >= mRenderSteps.size())
    {
        RIO_ASSERT(false);
        return;
    }

    mRenderSteps[render_step_idx].mDrawMethods.insert(draw_method);
}

void Layer::addDrawMethodToAll(const DrawMethod& draw_method)
{
    for (RenderStep& render_step : mRenderSteps)
        render_step.mDrawMethods.insert(draw_method);
}

void Layer::clearDrawMethods(u32 render_step_idx)
{
    if (render_step_idx >= mRenderSteps.size())
    {
        RIO_ASSERT(false);
        return;
    }

    mRenderSteps[render_step_idx].mDrawMethods.clear();
}

void Layer::clearDrawMethodsAll()
{
    for (RenderStep& render_step : mRenderSteps)
        render_step.mDrawMethods.clear();
}

} }
