#ifndef RIO_GFX_LYR_DRAWABLE_H
#define RIO_GFX_LYR_DRAWABLE_H

#include <misc/rio_Types.h>

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

public:
    // Draw method function pointer type.
    typedef void (IDrawable::*DrawMethod)(const DrawInfo&);
};

class Renderer;

class DrawMethod
{
    // Class for holding a draw method function pointer for use with the Renderer.

public:
    DrawMethod(IDrawable* obj_ptr, IDrawable::DrawMethod func_ptr, s32 priority = 0);

    template <typename T>
    DrawMethod(T* obj_ptr, void (T::*func_ptr)(const DrawInfo&), s32 priority = 0)
        : DrawMethod(static_cast<IDrawable*>(obj_ptr), static_cast<IDrawable::DrawMethod>(func_ptr), priority)
    {
    }

    bool operator<(const DrawMethod& rhs) const;

private:
    IDrawable*              mObjPtr;    // Draw method owner object pointer.
    IDrawable::DrawMethod   mFuncPtr;   // Draw method function pointer.
    const s32               mPriority;  // Priority of this draw method. (Smaller value = drawn later)

    friend class Renderer;
};

} }

#endif // RIO_GFX_LYR_DRAWABLE_H
