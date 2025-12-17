#ifndef RIO_GFX_LYR_DRAWABLE_H
#define RIO_GFX_LYR_DRAWABLE_H

#include <misc/rio_Types.h>

#include <functional>
#include <utility>
#include <concepts>

namespace rio { namespace lyr {

class Layer;

struct DrawInfo
{
    const Layer&    parent_layer;       // Reference to the layer the drawable object's draw method belongs to.
    u32             render_step_idx;    // Current render step of the layer the drawable object's draw method belongs to.
};

class IDrawable
{
    // Base interface for a drawable object, that is, an object with "draw methods".
};

class DrawMethod
{
    // Class for holding a draw method function pointer for use with the Renderer.

public:
    DrawMethod() = default;

    template <class T>
    DrawMethod(T* obj, void (T::*mf)(const DrawInfo&), s32 priority = 0)
        : mPriority(priority)
        , mFn([obj, mf](const DrawInfo& di) {
            (obj->*mf)(di);
          })
    {
    }

    void operator()(const DrawInfo& di) const
    {
        mFn(di);
    }

    bool operator<(const DrawMethod& rhs) const
    {
        return mPriority > rhs.mPriority;
    }

private:
    s32 mPriority{};
    std::function<void(const DrawInfo&)> mFn;
};

} }

#endif // RIO_GFX_LYR_DRAWABLE_H
