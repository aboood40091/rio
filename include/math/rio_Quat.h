#ifndef RIO_MATH_QUAT_H
#define RIO_MATH_QUAT_H

#include <math/rio_MathTypes.h>

namespace rio {

template <typename T>
struct Quat : BaseQuat<T>
{
    typedef Quat<T> Self;
    typedef T ValueType;

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const { return !operator==(v); }
};

typedef Quat<f32> Quatf;

}

//#include <math/impl/rio_QuatImpl.h>

#endif // RIO_MATH_QUAT_H
