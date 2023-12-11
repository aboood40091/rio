#ifndef RIO_GFX_LYR_LAYER_H
#define RIO_GFX_LYR_LAYER_H

#include <gfx/lyr/rio_Drawable.h>
#include <gfx/rio_Color.h>
#include <misc/rio_BitFlag.h>

#include <set>
#include <vector>

namespace rio {

class Camera;
class Projection;
class Window;

}

namespace rio { namespace lyr {

class RenderStep
{
    // Class representing a "render step", that is, a set of draw methods
    // for helping with organizing layers.

public:
    RenderStep(const char* name)
        : mName(name)
    {
    }

private:
    const char* mName;
    std::multiset<DrawMethod> mDrawMethods;

    friend class Layer;
    friend class Renderer;
};

class Layer
{
    // Class for separating the rendering process into several phases, i.e., “layers”.
    // Layers can intersect or be made to strictly appear on top of each other (regardless of objects' Z ordering).

private:
    struct Comparator
    {
        bool operator()(const Layer* lhs, const Layer* rhs) const
        {
            return *lhs < *rhs;
        }
    };

public:
    typedef std::multiset<Layer*, Comparator> List;
    typedef List::iterator iterator;

    static Layer* peelIterator(List::iterator& itr)
    {
        return const_cast<Layer*>(*itr);
    }

    static const Layer* peelIterator(const List::iterator& itr)
    {
        return *itr;
    }

public:
    // Get the default camera (identity)
    static const Camera& defaultCamera();
    // Get the default projection (ortho projection with Window size, center at (0,0), near as -1000 and far as 1000)
    static const Projection& defaultProjection();

private:
    static Projection& defaultProjection_();

#if RIO_IS_WIN
    static void onResize_(s32 width, s32 height);
#endif // RIO_IS_WIN
    friend class ::rio::Window;

public:
    Layer(const char* name, s32 priority);
    virtual ~Layer();

public:
    bool operator<(const Layer& rhs) const;

    // Get the layer's name.
    const char* name() const { return mName; }

    // Get the layer's priority.
    s32 priority() const { return mPriority; }

    // Get the active camera for the layer.
    const Camera* camera() const { return mpCamera; }
    // Get the active projection for the layer.
    const Projection* projection() const { return mpProjection; }

    // Set the active camera for the layer.
    void setCamera(const Camera* camera);
    // Set the active projection for the layer.
    void setProjection(const Projection* projection);

    // Enable color clear prior to rendering the layer and set its value.
    void setClearColor(const Color4f& color);

    // Enable depth clear prior to rendering the layer and set its value.
    void setClearDepth(f32 depth = 1.0f);
    // Enable stencil clear prior to rendering the layer and set its value.
    void setClearStencil(u8 stencil = 0);
    // Enable depth and stencil clear prior to rendering the layer and set their values.
    void setClearDepthStencil(f32 depth = 1.0f, u8 stencil = 0);

    // Set the viewport to use when drawing this layer
    void setViewport(s32 x, s32 y, u32 width, u32 height, f32 near = 0.0f, f32 far = 1.0f, s32 frame_buffer_height = -1);
    // Set the scissor box to use when drawing this layer
    void setScissor(s32 x, s32 y, u32 width, u32 height, s32 frame_buffer_height = -1);

    // Disable color clear.
    void resetClearColor();

    // Disable depth clear.
    void resetClearDepth();
    // Disable stencil clear.
    void resetClearStencil();
    // Disable depth and stencil clear.
    void resetClearDepthStencil();

    // Reset the viewport to use when drawing this layer (to Window size)
    void resetViewport();
    // Reset the scissor box to use when drawing this layer (to Window size)
    void resetScissor();

    // Create a render step with the given name.
    void addRenderStep(const char* name);
    // Remove all render steps.
    void clearRenderSteps();

    // Add a draw method to a render step by index.
    void addDrawMethod(u32 render_step_idx, const DrawMethod& draw_method);
    // Add a draw method to all render steps.
    void addDrawMethodToAll(const DrawMethod& draw_method);

    // Remove the draw methods of a render step by index.
    void clearDrawMethods(u32 render_step_idx);
    // Remove the draw methods of all render steps.
    void clearDrawMethodsAll();

    /*
        TODO:

    // Get index of render step by name.
    s32 getRenderStepIndex(const char* name) const;
    // Get name of render step by index.
    const char* getRenderStepName(u32 index) const;
    u32 getRenderStepCount() const;

    // Add a draw method to a render step by name.
    bool addDrawMethod(const char* render_step_name, const DrawMethod& draw_method);

    // Remove all draw methods of a render step by name.
    void clearDrawMethods(const char* render_step_name);
    */

protected:
    enum Flags
    {
        FLAGS_CLEAR_COLOR_BUFFER            = (1 << 0),
        FLAGS_CLEAR_DEPTH_BUFFER            = (1 << 1),
        FLAGS_CLEAR_STENCIL_BUFFER          = (1 << 2),

        FLAGS_CLEAR_DEPTH_STENCIL_BUFFER    = FLAGS_CLEAR_DEPTH_BUFFER | FLAGS_CLEAR_STENCIL_BUFFER,

        FLAGS_SET_VIEWPORT                  = (1 << 3),
        FLAGS_SET_SCISSOR                   = (1 << 4)
    };

    const char*             mName;          // Layer name.
    const s32               mPriority;      // Layer render priority. (Smaller value = drawn later)

    const Camera*           mpCamera;       // Active camera pointer.
    const Projection*       mpProjection;   // Active projection pointer.

    Color4f                 mClearColor;    // Color clear value.
    f32                     mClearDepth;    // Depth clear value.
    u8                      mClearStencil;  // Stencil clear value.

    struct
    {
        s32 x;
        s32 y;
        u32 width;
        u32 height;
        f32 near;
        f32 far;
        s32 frame_buffer_height;
    } mViewport;

    struct
    {
        s32 x;
        s32 y;
        u32 width;
        u32 height;
        s32 frame_buffer_height;
    } mScissor;

    BitFlag8                mFlags;         // Flags.

    std::vector<RenderStep> mRenderSteps;   // Layer render steps.

    friend class Renderer;
};

} }

#endif // RIO_GFX_LYR_LAYER_H
