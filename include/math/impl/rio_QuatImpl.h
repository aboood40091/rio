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
inline void
Quat<f32>::setOppositeDir(const Quatf& q)
{
    Quatf*       const pSelf = this;
    const Quatf* const pQ    = &q;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pQ into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pQ]), 0, 0" : [v0] "=f"(v0) : [pQ] "r"(pQ));

    // Negate ps(v0)
    asm volatile ("ps_neg %[v0], %[v0]" : [v0] "+f"(v0));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pQ+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pQ]), 0, 0" : [v0] "=f"(v0) : [pQ] "r"(pQ));

    // Negate ps(v0)
    asm volatile ("ps_neg %[v0], %[v0]" : [v0] "+f"(v0));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

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
inline void
Quat<f32>::setAdd(const Quatf& a, const Quatf& b)
{
    Quatf*       const pSelf = this;
    const Quatf* const pA    = &a;
    const Quatf* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Add ps(v1) to ps(v0)
    asm volatile ("ps_add %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pA+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Add ps(v1) to ps(v0)
    asm volatile ("ps_add %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

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
inline void
Quat<f32>::setSub(const Quatf& a, const Quatf& b)
{
    Quatf*       const pSelf = this;
    const Quatf* const pA    = &a;
    const Quatf* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Subtract ps(v1) from ps(v0)
    asm volatile ("ps_sub %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pA+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Subtract ps(v1) from ps(v0)
    asm volatile ("ps_sub %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

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
inline void
Quat<f32>::setScale(const Quatf& q, f32 s)
{
    Quatf*       const pSelf = this;
    const Quatf* const pQ    = &q;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pQ into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pQ]), 0, 0" : [v0] "=f"(v0) : [pQ] "r"(pQ));

    // Multiply ps(v0) by s
    asm volatile ("ps_muls0 %[v0], %[v0], %[s]" : [v0] "+f"(v0) : [s] "f"(s));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pQ+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pQ]), 0, 0" : [v0] "=f"(v0) : [pQ] "r"(pQ));

    // Multiply ps(v0) by s
    asm volatile ("ps_muls0 %[v0], %[v0], %[s]" : [v0] "+f"(v0) : [s] "f"(s));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

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
inline void
Quat<f32>::setScaleInv(const Quatf& q, f32 s)
{
    Quatf*       const pSelf = this;
    const Quatf* const pQ    = &q;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pQ into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pQ]), 0, 0" : [v0] "=f"(v0) : [pQ] "r"(pQ));

    // Load s into ps(v1)
    asm volatile ("ps_merge00 %[v1], %[s], %[s]" : [v1] "=f"(v1) : [s] "f"(s));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pQ+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pQ]), 0, 0" : [v0] "=f"(v0) : [pQ] "r"(pQ));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

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
inline f32
Quat<f32>::squaredLength() const
{
    const Quatf* const pSelf = this;

    // Temporary variables
    f32 v0;
    f32 v2;
    f32 v3;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Multiply ps(v0) by itself
    asm volatile ("ps_mul %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    // Add ps1(v0) to ps0(v0) and store result in ps(v2)
    asm volatile ("ps_sum0 %[v2], %[v0], %[v0], %[v0]" : [v2] "=f"(v2) : [v0] "f"(v0));

    // Load 2 floats from pSelf+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Multiply ps(v0) by itself
    asm volatile ("ps_mul %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    // Add ps1(v0) to ps0(v0) and store result in ps(v3)
    asm volatile ("ps_sum0 %[v3], %[v0], %[v0], %[v0]" : [v3] "=f"(v3) : [v0] "f"(v0));

    return v2 + v3;
}

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
inline f32
Quat<f32>::dot(const Quatf& q) const
{
    const Quatf* const pSelf = this;
    const Quatf* const pQ    = &q;

    // Temporary variables
    f32 v0;
    f32 v1;
    f32 v2;
    f32 v3;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Load 2 floats from pQ into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pQ]), 0, 0" : [v1] "=f"(v1) : [pQ] "r"(pQ));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Add ps1(v0) to ps0(v0) and store result in ps(v2)
    asm volatile ("ps_sum0 %[v2], %[v0], %[v0], %[v0]" : [v2] "=f"(v2) : [v0] "f"(v0));

    // Load 2 floats from pSelf+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Load 2 floats from pQ+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pQ]), 0, 0" : [v1] "=f"(v1) : [pQ] "r"(pQ));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Add ps1(v0) to ps0(v0) and store result in ps(v3)
    asm volatile ("ps_sum0 %[v3], %[v0], %[v0], %[v0]" : [v3] "=f"(v3) : [v0] "f"(v0));

    return v2 + v3;
}

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
inline void
Quat<f32>::setScaleAdd(const Quatf& a, f32 s, const Quatf& b)
{
    Quatf*       const pSelf = this;
    const Quatf* const pA    = &a;
    const Quatf* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v2;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v2)
    asm volatile ("psq_l %[v2], 0(%[pB]), 0, 0" : [v2] "=f"(v2) : [pB] "r"(pB));

    // Multiply ps(v0) by s and add ps(v2)
    asm volatile ("ps_madds0 %[v0], %[v0], %[s], %[v2]" : [v0] "+f"(v0) : [s] "f"(s), [v2] "f"(v2));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pA+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB+8 into ps(v2)
    asm volatile ("psq_l %[v2], 8(%[pB]), 0, 0" : [v2] "=f"(v2) : [pB] "r"(pB));

    // Multiply ps(v0) by s and add ps(v2)
    asm volatile ("ps_madds0 %[v0], %[v0], %[s], %[v2]" : [v0] "+f"(v0) : [s] "f"(s), [v2] "f"(v2));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

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
