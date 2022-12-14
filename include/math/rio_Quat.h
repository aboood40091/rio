#ifndef RIO_MATH_QUAT_H
#define RIO_MATH_QUAT_H

#include <math/rio_MathTypes.h>

namespace rio {

template <typename T>
struct Quat : BaseQuat<T>
{
    typedef Quat<T> Self;
    typedef T ValueType;

    bool operator==(const Self& q) const;
    bool operator!=(const Self& q) const { return !operator==(q); }

    Self operator-() const;
    void setOppositeDir(const Self& q);

    Self operator+(const Self& q) const;
    void setAdd(const Self& a, const Self& b);

    Self operator-(const Self& q) const;
    void setSub(const Self& a, const Self& b);

    Self operator*(const Self& q) const;
    void setMul(const Self& a, const Self& b);

    Self operator*(T s) const;
    void setScale(const Self& q, T s);

    Self operator/(T s) const;
    void setScaleInv(const Self& q, T s);

    Self& operator+=(const Self& q);
    Self& operator-=(const Self& q);
    Self& operator*=(const Self& q);

    Self& operator*=(T s);
    Self& operator/=(T s);

    void set(T w_, T x_, T y_, T z_);

    T squaredLength() const { return dot(*this); }
    f32 length() const;

    T dot(const Self& q) const;

    f32 normalize() { return setNormalized(*this); }

    Self normalized() const;
    f32 setNormalized(const Self& q);

    Self multAdd(const Self& c, const Self& b) const;
    void setMultAdd(const Self& a, const Self& c, const Self& b);

    Self scaleAdd(T s, const Self& b) const;
    void setScaleAdd(const Self& a, T s, const Self& b);

    Self lerp(const Self& b, T r) const;
    void setLerp(const Self& a, const Self& b, T r);

    Self slerp(const Self& b, T r) const;
    void setSlerp(const Self& a, const Self& b, T r);
};

typedef Quat<f32> Quatf;

}

#include <math/impl/rio_QuatImpl.h>

#endif // RIO_MATH_QUAT_H
