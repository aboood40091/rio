#include <gfx/lyr/rio_Drawable.h>

namespace rio { namespace lyr {

DrawMethod::DrawMethod(IDrawable* obj_ptr, IDrawable::DrawMethod func_ptr, s32 priority)
    : mObjPtr(obj_ptr)
    , mFuncPtr(func_ptr)
    , mPriority(priority)
{
}

bool DrawMethod::operator<(const DrawMethod& rhs) const
{
    return mPriority > rhs.mPriority;
}

} }
