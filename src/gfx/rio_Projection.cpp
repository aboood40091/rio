#include <gfx/rio_Projection.h>

namespace rio {

PerspectiveProjection::PerspectiveProjection()
    : mNear(1.0f)
    , mFar(10000.0f)
    , mAspect(4.f/3.f)
{
    mOffset.set(0.0f, 0.0f);
    setFovy(Mathf::deg2rad(45));
}

PerspectiveProjection::PerspectiveProjection(f32 near, f32 far, f32 fovy, f32 aspect)
    : mNear(near)
    , mFar(far)
    , mAspect(aspect)
{
    mOffset.set(0.0f, 0.0f);
    setFovy(fovy);
}

void PerspectiveProjection::set(f32 near, f32 far, f32 fovy, f32 aspect)
{
    setNear(near);
    setFar(far);
    setFovy(fovy);
    setAspect(aspect);
}

void PerspectiveProjection::setFovx(f32 fovx)
{
    mAspect = std::tan(fovx * 0.5f) / mFovyTan;
    mDirty = true;
}

void PerspectiveProjection::setFovy_(f32 fovy)
{
    mAngle = fovy;
    mFovySin = std::sin(fovy * 0.5f);
    mFovyCos = std::cos(fovy * 0.5f);
    mFovyTan = std::tan(fovy * 0.5f);
    mDirty = true;
}

void PerspectiveProjection::updateMatrix_() const
{
    f32 clip_height = mNear * 2 * mFovyTan;
    f32 clip_width  = clip_height * mAspect;

    f32 center_x = clip_width * mOffset.x;
    f32 center_y = clip_height * mOffset.y;

    clip_height *= 0.5f;
    clip_width  *= 0.5f;

    f32 top    =  clip_height + center_y;
    f32 bottom = -clip_height + center_y;

    f32 left   = -clip_width  + center_x;
    f32 right  =  clip_width  + center_x;

    f32 inv_size = 1.0f / (right - left);

    mMatrix.m[0][0] = mNear * 2 * inv_size;
    mMatrix.m[0][1] = 0.0f;
    mMatrix.m[0][2] = (right + left) * inv_size;
    mMatrix.m[0][3] = 0.0f;

    inv_size = 1.0f / (top - bottom);

    mMatrix.m[1][0] = 0.0f;
    mMatrix.m[1][1] = mNear * 2 * inv_size;
    mMatrix.m[1][2] = (top + bottom) * inv_size;
    mMatrix.m[1][3] = 0.0f;

    inv_size = 1.0f / (mFar - mNear);

    mMatrix.m[2][0] = 0.0f;
    mMatrix.m[2][1] = 0.0f;
    mMatrix.m[2][2] = -(mFar + mNear) * inv_size;
    mMatrix.m[2][3] = -(mFar * 2 * mNear) * inv_size;

    mMatrix.m[3][0] = 0.0f;
    mMatrix.m[3][1] = 0.0f;
    mMatrix.m[3][2] = -1.0f;
    mMatrix.m[3][3] = 0.0f;
}

OrthoProjection::OrthoProjection()
    : mNear(0.0f)
    , mFar(1.0f)
{
    setTBLR(0.5f, -0.5f, -0.5f, 0.5f);
}

OrthoProjection::OrthoProjection(f32 near, f32 far, f32 top, f32 bottom, f32 left, f32 right)
    : mNear(near)
    , mFar(far)
{
    setTBLR(top, bottom, left, right);
}

void OrthoProjection::set(f32 near, f32 far, f32 top, f32 bottom, f32 left, f32 right)
{
    setNear(near);
    setFar(far);
    setTBLR(top, bottom, left, right);
}

void OrthoProjection::updateMatrix_() const
{
    const BaseVec3f& v = { .x = (mLeft + mRight ) / 2.0f,
                           .y = (mTop  + mBottom) / 2.0f,
                           .z = (mNear + mFar   ) / 2.0f };

    f32 w_2 = (mRight - mLeft) / 2.0f;
    mMatrix.m[0][0] = 1.0f / w_2;
    mMatrix.m[0][1] = 0.0f;
    mMatrix.m[0][2] = 0.0f;

    f32 h_2 = (mTop - mBottom) / 2.0f;
    mMatrix.m[1][0] = 0.0f;
    mMatrix.m[1][1] = 1.0f / h_2;
    mMatrix.m[1][2] = 0.0f;

    f32 l_2 = (mNear - mFar) / 2.0f;
    mMatrix.m[2][0] = 0.0f;
    mMatrix.m[2][1] = 0.0f;
    mMatrix.m[2][2] = 1.0f / l_2;

    mMatrix.m[0][3] = -v.x / w_2;
    mMatrix.m[1][3] = -v.y / h_2;
    mMatrix.m[2][3] =  v.z / l_2;

    mMatrix.m[3][0] = 0.0f;
    mMatrix.m[3][1] = 0.0f;
    mMatrix.m[3][2] = 0.0f;
    mMatrix.m[3][3] = 1.0f;
}

}
