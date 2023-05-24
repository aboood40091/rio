#ifndef RIO_MATH_QUAT_IMPL_H
#define RIO_MATH_QUAT_IMPL_H

// This file is already included in rio_Quat.h
//#include <math/rio_Quat.h>

#include <math/rio_Math.h>

namespace rio {

template <typename T>
inline bool
Quat<T>::operator==(const Self& q) const
{
    return this->x == q.x && this->y == q.y && this->z == q.z && this->w == q.w;
}

template <typename T>
inline Quat<T>
Quat<T>::operator-() const
{
    Self o;
    o.setOppositeDir(*this);
    return o;
}

template <typename T>
inline void
Quat<T>::setOppositeDir(const Self& q)
{
    this->x = -q.x;
    this->y = -q.y;
    this->z = -q.z;
    this->w = -q.w;
}

#if RIO_IS_CAFE

template <>
void
Quat<f32>::setOppositeDir(const Quatf& q);

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>
Quat<T>::operator+(const Self& q) const
{
    Self o;
    o.setAdd(*this, q);
    return o;
}

template <typename T>
inline void
Quat<T>::setAdd(const Self& a, const Self& b)
{
    this->x = a.x + b.x;
    this->y = a.y + b.y;
    this->z = a.z + b.z;
    this->w = a.w + b.w;
}

#if RIO_IS_CAFE

template <>
void
Quat<f32>::setAdd(const Quatf& a, const Quatf& b);

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>
Quat<T>::operator-(const Self& q) const
{
    Self o;
    o.setSub(*this, q);
    return o;
}

template <typename T>
inline void
Quat<T>::setSub(const Self& a, const Self& b)
{
    this->x = a.x - b.x;
    this->y = a.y - b.y;
    this->z = a.z - b.z;
    this->w = a.w - b.w;
}

#if RIO_IS_CAFE

template <>
void
Quat<f32>::setSub(const Quatf& a, const Quatf& b);

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>
Quat<T>::operator*(const Self& q) const
{
    Self o;
    o.setMul(*this, q);
    return o;
}

template <typename T>
inline Quat<T>
Quat<T>::operator*(T s) const
{
    Self o;
    o.setScale(*this, s);
    return o;
}

template <typename T>
inline void
Quat<T>::setScale(const Self& q, T s)
{
    this->x = q.x * s;
    this->y = q.y * s;
    this->z = q.z * s;
    this->w = q.w * s;
}

#if RIO_IS_CAFE

template <>
void
Quat<f32>::setScale(const Quatf& q, f32 s);

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>
Quat<T>::operator/(T s) const
{
    Self o;
    o.setScaleInv(*this, s);
    return o;
}

template <typename T>
inline void
Quat<T>::setScaleInv(const Self& q, T s)
{
    this->x = q.x / s;
    this->y = q.y / s;
    this->z = q.z / s;
    this->w = q.w / s;
}

#if RIO_IS_CAFE

template <>
void
Quat<f32>::setScaleInv(const Quatf& q, f32 s);

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>&
Quat<T>::operator+=(const Self& q)
{
    this->setAdd(*this, q);
    return *this;
}

template <typename T>
inline Quat<T>&
Quat<T>::operator-=(const Self& q)
{
    this->setSub(*this, q);
    return *this;
}

template <typename T>
inline Quat<T>&
Quat<T>::operator*=(const Self& q)
{
    this->setMul(*this, q);
    return *this;
}

template <typename T>
inline Quat<T>&
Quat<T>::operator*=(T s)
{
    this->setScale(*this, s);
    return *this;
}

template <typename T>
inline Quat<T>&
Quat<T>::operator/=(T s)
{
    this->setScaleInv(*this, s);
    return *this;
}

template <typename T>
inline void
Quat<T>::set(T w_, T x_, T y_, T z_)
{
    this->w = w_;
    this->x = x_;
    this->y = y_;
    this->z = z_;
}

#if RIO_IS_CAFE

template <>
f32
Quat<f32>::squaredLength() const;

#endif // RIO_IS_CAFE

template <typename T>
inline f32
Quat<T>::length() const
{
    return Mathf::sqrt(squaredLength());
}

template <typename T>
inline T
Quat<T>::dot(const Self& q) const
{
    return this->x * q.x + this->y * q.y + this->z * q.z + this->w * q.w;
}

#if RIO_IS_CAFE

template <>
f32
Quat<f32>::dot(const Quatf& q) const;

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>
Quat<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0 || this->z != 0 || this->w != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return (*this) * inv_length;
}

template <typename T>
inline f32
Quat<T>::setNormalized(const Self& q)
{
    RIO_ASSERT(q.x != 0 || q.y != 0 || q.z != 0 || q.w != 0);

    f32 sq_length = q.squaredLength();
    f32 inv_length = Mathf::rsqrt(sq_length);

    setScale(q, inv_length);

    return sq_length * inv_length;
}

template <typename T>
inline Quat<T>
Quat<T>::scaleAdd(T s, const Self& b) const
{
    Self o;
    o.setScaleAdd(*this, s, b);
    return o;
}

template <typename T>
inline void
Quat<T>::setScaleAdd(const Self& a, T s, const Self& b)
{
    this->x = a.x * s + b.x;
    this->y = a.y * s + b.y;
    this->z = a.z * s + b.z;
    this->w = a.w * s + b.w;
}

#if RIO_IS_CAFE

template <>
void
Quat<f32>::setScaleAdd(const Quatf& a, f32 s, const Quatf& b);

#endif // RIO_IS_CAFE

template <typename T>
inline Quat<T>
Quat<T>::slerp(const Self& b, T r) const
{
    Self o;
    o.setSlerp(*this, b, r);
    return o;
}

template <typename T>
inline void
Quat<T>::setSlerp(const Self& a, const Self& b, T r)
{
    T cos_v = a.dot(b);

    T r0;
    T r1 = cos_v >= 0.0f ? 1.0f : -1.0f;

    cos_v = Math<T>::abs(cos_v);
    if (cos_v <= T(0.99999))
    {
        T rad = std::acos(cos_v);
        T sin_v = std::sin(rad);
        r0 = std::sin((T(1) - r) * rad) / sin_v;
        r1 *= std::sin(r * rad) / sin_v;
    }
    else
    {
        r0 = 1.0f - r;
        r1 *= r;
    }

    // *this = a * r0 + b * r1
    setScaleAdd(a, r0, b * r1);
}

}

#endif // RIO_MATH_QUAT_IMPL_H
