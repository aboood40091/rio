#include <gfx/rio_Camera.h>

namespace rio {

void LookAtCamera::getMatrix(BaseMtx34f* dst) const
{
    RIO_ASSERT(dst);
    RIO_ASSERT(mPos != mAt);

    const Vector3f& dir = (mPos - mAt).normalized();
    const Vector3f& right = (mUp.cross(dir)).normalized();
    const Vector3f& up = dir.cross(right);

    dst->m[0][0] = right.x;
    dst->m[0][1] = right.y;
    dst->m[0][2] = right.z;
    dst->m[0][3] = -right.dot(mPos);

    dst->m[1][0] = up.x;
    dst->m[1][1] = up.y;
    dst->m[1][2] = up.z;
    dst->m[1][3] = -up.dot(mPos);

    dst->m[2][0] = dir.x;
    dst->m[2][1] = dir.y;
    dst->m[2][2] = dir.z;
    dst->m[2][3] = -dir.dot(mPos);
}

void OrthoCamera::getMatrix(BaseMtx34f* dst) const
{
    RIO_ASSERT(dst);

    dst->m[0][0] = mZoomScale;
    dst->m[0][1] = 0.0f;
    dst->m[0][2] = 0.0f;
    dst->m[0][3] = -mPos.x * mZoomScale;

    dst->m[1][0] = 0.0f;
    dst->m[1][1] = mZoomScale;
    dst->m[1][2] = 0.0f;
    dst->m[1][3] = -mPos.y * mZoomScale;

    dst->m[2][0] = 0.0f;
    dst->m[2][1] = 0.0f;
    dst->m[2][2] = 1.0f;
    dst->m[2][3] = 0.0f;
}

}
