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
    f32 v0 = (mNear + mNear) * mFovyTan;
    f32 v1 = v0 * mAspect;

    f32 v2 = v1 * mOffset.x;
    f32 v3 = v0 * mOffset.y;

    f32 v4 = v0 * 0.5f + v3;

    v3 = v3 - v0 * 0.5f;
    f32 v5 = v2 - v1 * 0.5f;

    v2 = v1 * 0.5f + v2;

    f32 v6 = 1.0f / (v2 - v5);
    mMatrix.m[0][0] = (mNear + mNear) * v6;
    mMatrix.m[0][1] = 0.0f;
    mMatrix.m[0][2] = (v2 + v5) * v6;
    mMatrix.m[0][3] = 0.0f;

    f32 v7 = 1.0f / (v4 - v3);
    mMatrix.m[1][0] = 0.0f;
    mMatrix.m[1][1] = (mNear + mNear) * v7;
    mMatrix.m[1][2] = (v4 + v3) * v7;
    mMatrix.m[1][3] = 0.0f;

    f32 v8 = 1.0f / (mFar - mNear);
    mMatrix.m[2][0] = 0.0f;
    mMatrix.m[2][1] = 0.0f;
    mMatrix.m[2][2] = -(mFar + mNear) * v8;
    mMatrix.m[2][3] = -((mFar + mFar) * mNear) * v8;

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
