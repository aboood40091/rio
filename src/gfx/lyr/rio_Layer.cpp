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

const Camera& Layer::defaultCamera()
{
    static const IdentityCamera cDefaultCamera;
    return cDefaultCamera;
}

const Projection& Layer::defaultProjection()
{
    return defaultProjection_();
}

Projection& Layer::defaultProjection_()
{
    Window* const window = Window::instance();
    RIO_ASSERT(window);

    const f32 window_width_2 = window->getWidth() * 0.5f;
    const f32 window_height_2 = window->getHeight() * 0.5f;

    static OrthoProjection sDefaultProjection(-1000.0f, 1000.0f, window_height_2, -window_height_2, -window_width_2, window_width_2);
    return sDefaultProjection;
}

#if RIO_IS_WIN

void Layer::onResize_(s32 width, s32 height)
{
    const f32 window_width_2 = width * 0.5f;
    const f32 window_height_2 = height * 0.5f;

    static_cast<OrthoProjection&>(defaultProjection_()).setTBLR(window_height_2, -window_height_2, -window_width_2, window_width_2);
}

#endif // RIO_IS_WIN

Layer::Layer(const char* name, s32 priority)
    : mName(name)
    , mPriority(priority)
    , mClearColor{ Color4f::cBlack }
    , mClearDepth(1.0f)
    , mClearStencil(0)
    , mFlags(0)
{
    mpCamera = &defaultCamera();
    mpProjection = &defaultProjection();
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
    mFlags.set(FLAGS_CLEAR_COLOR_BUFFER);
}

void Layer::setClearDepth(f32 depth)
{
    mClearDepth = depth;
    mFlags.set(FLAGS_CLEAR_DEPTH_BUFFER);
}

void Layer::setClearStencil(u8 stencil)
{
    mClearStencil = stencil;
    mFlags.set(FLAGS_CLEAR_STENCIL_BUFFER);
}

void Layer::setClearDepthStencil(f32 depth, u8 stencil)
{
    mClearDepth = depth;
    mClearStencil = stencil;
    mFlags.set(FLAGS_CLEAR_DEPTH_STENCIL_BUFFER);
}

void Layer::setViewport(s32 x, s32 y, u32 width, u32 height, f32 near, f32 far, s32 frame_buffer_height)
{
    mViewport.x = x;
    mViewport.y = y;
    mViewport.width = width;
    mViewport.height = height;
    mViewport.near = near;
    mViewport.far = far;
    mViewport.frame_buffer_height = frame_buffer_height;
    mFlags.set(FLAGS_SET_VIEWPORT);
}

void Layer::setScissor(s32 x, s32 y, u32 width, u32 height, s32 frame_buffer_height)
{
    mScissor.x = x;
    mScissor.y = y;
    mScissor.width = width;
    mScissor.height = height;
    mScissor.frame_buffer_height = frame_buffer_height;
    mFlags.set(FLAGS_SET_SCISSOR);
}

void Layer::resetClearColor()
{
    mFlags.reset(FLAGS_CLEAR_COLOR_BUFFER);
}

void Layer::resetClearDepth()
{
    mFlags.reset(FLAGS_CLEAR_DEPTH_BUFFER);
}

void Layer::resetClearStencil()
{
    mFlags.reset(FLAGS_CLEAR_STENCIL_BUFFER);
}

void Layer::resetClearDepthStencil()
{
    mFlags.reset(FLAGS_CLEAR_DEPTH_STENCIL_BUFFER);
}

void Layer::resetViewport()
{
    mFlags.reset(FLAGS_SET_VIEWPORT);
}

void Layer::resetScissor()
{
    mFlags.reset(FLAGS_SET_SCISSOR);
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
