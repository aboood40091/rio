#include <gfx/rio_Camera.h>

namespace rio {

void LookAtCamera::getMatrix(BaseMtx34f* dst) const
{
    RIO_ASSERT(dst);
    RIO_ASSERT(mPos != mAt);

    Vector3f a;
    a.setNormalized(mPos - mAt);

    Vector3f b;
    b.setNormalized(mUp.cross(a));

    Vector3f c;
    c.setCross(a, b);

    dst->m[0][0] = b.x;
    dst->m[0][1] = b.y;
    dst->m[0][2] = b.z;
    dst->m[0][3] = -b.dot(mPos);

    dst->m[1][0] = c.x;
    dst->m[1][1] = c.y;
    dst->m[1][2] = c.z;
    dst->m[1][3] = -c.dot(mPos);

    dst->m[2][0] = a.x;
    dst->m[2][1] = a.y;
    dst->m[2][2] = a.z;
    dst->m[2][3] = -a.dot(mPos);
}

}
