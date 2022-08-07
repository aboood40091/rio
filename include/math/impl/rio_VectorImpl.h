#ifndef RIO_MATH_VECTOR_IMPL_H
#define RIO_MATH_VECTOR_IMPL_H

// This file is already included in rio_Vector.h
//#include <math/rio_Vector.h>

#include <math/rio_Math.h>

namespace rio {

template <typename T>
inline bool
Vector2<T>::operator==(const Self& v) const
{
    return this->x == v.x && this->y == v.y;
}

template <typename T>
inline Vector2<T>
Vector2<T>::operator-() const
{
    Self o;
    o.setOppositeDir(*this);
    return o;
}

template <typename T>
inline void
Vector2<T>::setOppositeDir(const Self& v)
{
    this->x = -v.x;
    this->y = -v.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setOppositeDir(const Vector2f& v)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pV    = &v;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Negate ps(v0)
    asm volatile ("ps_neg %[v0], %[v0]" : [v0] "+f"(v0));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::operator+(const Self& v) const
{
    Self o;
    o.setAdd(*this, v);
    return o;
}

template <typename T>
inline void
Vector2<T>::setAdd(const Self& a, const Self& b)
{
    this->x = a.x + b.x;
    this->y = a.y + b.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setAdd(const Vector2f& a, const Vector2f& b)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pA    = &a;
    const Vector2f* const pB    = &b;

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
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::operator-(const Self& v) const
{
    Self o;
    o.setSub(*this, v);
    return o;
}

template <typename T>
inline void
Vector2<T>::setSub(const Self& a, const Self& b)
{
    this->x = a.x - b.x;
    this->y = a.y - b.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setSub(const Vector2f& a, const Vector2f& b)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pA    = &a;
    const Vector2f* const pB    = &b;

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
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::operator*(const Self& v) const
{
    Self o;
    o.setMul(*this, v);
    return o;
}

template <typename T>
inline void
Vector2<T>::setMul(const Self& a, const Self& b)
{
    this->x = a.x * b.x;
    this->y = a.y * b.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setMul(const Vector2f& a, const Vector2f& b)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pA    = &a;
    const Vector2f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::operator/(const Self& v) const
{
    Self o;
    o.setDiv(*this, v);
    return o;
}

template <typename T>
inline void
Vector2<T>::setDiv(const Self& a, const Self& b)
{
    this->x = a.x / b.x;
    this->y = a.y / b.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setDiv(const Vector2f& a, const Vector2f& b)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pA    = &a;
    const Vector2f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::operator*(T s) const
{
    Self o;
    o.setScale(*this, s);
    return o;
}

template <typename T>
inline void
Vector2<T>::setScale(const Self& v, T s)
{
    this->x = v.x * s;
    this->y = v.y * s;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setScale(const Vector2f& v, f32 s)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pV    = &v;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Multiply ps(v0) by s
    asm volatile ("ps_muls0 %[v0], %[v0], %[s]" : [v0] "+f"(v0) : [s] "f"(s));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::operator/(T s) const
{
    Self o;
    o.setScaleInv(*this, s);
    return o;
}

template <typename T>
inline void
Vector2<T>::setScaleInv(const Self& v, T s)
{
    this->x = v.x / s;
    this->y = v.y / s;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setScaleInv(const Vector2f& v, f32 s)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pV    = &v;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Load s into ps(v1)
    asm volatile ("ps_merge00 %[v1], %[s], %[s]" : [v1] "=f"(v1) : [s] "f"(s));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>&
Vector2<T>::operator+=(const Self& v)
{
    this->setAdd(*this, v);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator-=(const Self& v)
{
    this->setSub(*this, v);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator*=(const Self& v)
{
    this->setMul(*this, v);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator/=(const Self& v)
{
    this->setDiv(*this, v);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator*=(T s)
{
    this->setScale(*this, s);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator/=(T s)
{
    this->setScaleInv(*this, s);
    return *this;
}

template <typename T>
inline void
Vector2<T>::set(T x_, T y_)
{
    this->x = x_;
    this->y = y_;
}

template <typename T>
inline f32
Vector2<T>::length() const
{
    return Mathf::sqrt(squaredLength());
}

template <typename T>
inline T
Vector2<T>::dot(const Self& v) const
{
    return this->x * v.x + this->y * v.y;
}

#if RIO_IS_CAFE

template <>
inline f32
Vector2<f32>::dot(const Vector2f& v) const
{
    const Vector2f* const pSelf = this;
    const Vector2f* const pV    = &v;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Load 2 floats from pV into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pV]), 0, 0" : [v1] "=f"(v1) : [pV] "r"(pV));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Add ps1(v0) to ps0(v0)
    asm volatile ("ps_sum0 %[v0], %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    return v0;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return {
        this->x * inv_length,
        this->y * inv_length
    };
}

template <typename T>
inline void
Vector2<T>::setNormalized(const Self& v)
{
    RIO_ASSERT(v.x != 0 || v.y != 0);

    f32 inv_length = Mathf::rsqrt(v.squaredLength());

    this->x = v.x * inv_length;
    this->y = v.y * inv_length;
}

template <typename T>
inline Vector2<T>
Vector2<T>::multAdd(const Self& c, const Self& b) const
{
    Self o;
    o.setMultAdd(*this, c, b);
    return o;
}

template <typename T>
inline void
Vector2<T>::setMultAdd(const Self& a, const Self& c, const Self& b)
{
    this->x = a.x * c.x + b.x;
    this->y = a.y * c.y + b.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setMultAdd(const Vector2f& a, const Vector2f& c, const Vector2f& b)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pA    = &a;
    const Vector2f* const pC    = &c;
    const Vector2f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;
    f32 v2;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pC into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pC]), 0, 0" : [v1] "=f"(v1) : [pC] "r"(pC));

    // Load 2 floats from pB into ps(v2)
    asm volatile ("psq_l %[v2], 0(%[pB]), 0, 0" : [v2] "=f"(v2) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1) and add ps(v2)
    asm volatile ("ps_madd %[v0], %[v0], %[v1], %[v2]" : [v0] "+f"(v0) : [v1] "f"(v1), [v2] "f"(v2));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::scaleAdd(T s, const Self& b) const
{
    Self o;
    o.setScaleAdd(*this, s, b);
    return o;
}

template <typename T>
inline void
Vector2<T>::setScaleAdd(const Self& a, T s, const Self& b)
{
    this->x = a.x * s + b.x;
    this->y = a.y * s + b.y;
}

#if RIO_IS_CAFE

template <>
inline void
Vector2<f32>::setScaleAdd(const Vector2f& a, f32 s, const Vector2f& b)
{
    Vector2f*       const pSelf = this;
    const Vector2f* const pA    = &a;
    const Vector2f* const pB    = &b;

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
}

#endif // RIO_IS_CAFE

template <typename T>
inline bool
Vector3<T>::operator==(const Self& v) const
{
    return this->x == v.x && this->y == v.y && this->z == v.z;
}

template <typename T>
inline Vector3<T>
Vector3<T>::operator-() const
{
    Self o;
    o.setOppositeDir(*this);
    return o;
}

template <typename T>
inline void
Vector3<T>::setOppositeDir(const Self& v)
{
    this->x = -v.x;
    this->y = -v.y;
    this->z = -v.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setOppositeDir(const Vector3f& v)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pV    = &v;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Negate ps(v0)
    asm volatile ("ps_neg %[v0], %[v0]" : [v0] "+f"(v0));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Handle z component
    pSelf->z = -pV->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::operator+(const Self& v) const
{
    Self o;
    o.setAdd(*this, v);
    return o;
}

template <typename T>
inline void
Vector3<T>::setAdd(const Self& a, const Self& b)
{
    this->x = a.x + b.x;
    this->y = a.y + b.y;
    this->z = a.z + b.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setAdd(const Vector3f& a, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pB    = &b;

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

    // Handle z component
    pSelf->z = pA->z + pB->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::operator-(const Self& v) const
{
    Self o;
    o.setSub(*this, v);
    return o;
}

template <typename T>
inline void
Vector3<T>::setSub(const Self& a, const Self& b)
{
    this->x = a.x - b.x;
    this->y = a.y - b.y;
    this->z = a.z - b.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setSub(const Vector3f& a, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pB    = &b;

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

    // Handle z component
    pSelf->z = pA->z - pB->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::operator*(const Self& v) const
{
    Self o;
    o.setMul(*this, v);
    return o;
}

template <typename T>
inline void
Vector3<T>::setMul(const Self& a, const Self& b)
{
    this->x = a.x * b.x;
    this->y = a.y * b.y;
    this->z = a.z * b.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setMul(const Vector3f& a, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Handle z component
    pSelf->z = pA->z * pB->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::operator/(const Self& v) const
{
    Self o;
    o.setDiv(*this, v);
    return o;
}

template <typename T>
inline void
Vector3<T>::setDiv(const Self& a, const Self& b)
{
    this->x = a.x / b.x;
    this->y = a.y / b.y;
    this->z = a.z / b.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setDiv(const Vector3f& a, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Handle z component
    pSelf->z = pA->z / pB->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::operator*(T s) const
{
    Self o;
    o.setScale(*this, s);
    return o;
}

template <typename T>
inline void
Vector3<T>::setScale(const Self& v, T s)
{
    this->x = v.x * s;
    this->y = v.y * s;
    this->z = v.z * s;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setScale(const Vector3f& v, f32 s)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pV    = &v;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Multiply ps(v0) by s
    asm volatile ("ps_muls0 %[v0], %[v0], %[s]" : [v0] "+f"(v0) : [s] "f"(s));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Handle z component
    pSelf->z = pV->z * s;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::operator/(T s) const
{
    Self o;
    o.setScaleInv(*this, s);
    return o;
}

template <typename T>
inline void
Vector3<T>::setScaleInv(const Self& v, T s)
{
    this->x = v.x / s;
    this->y = v.y / s;
    this->z = v.z / s;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setScaleInv(const Vector3f& v, f32 s)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pV    = &v;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Load s into ps(v1)
    asm volatile ("ps_merge00 %[v1], %[s], %[s]" : [v1] "=f"(v1) : [s] "f"(s));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Handle z component
    pSelf->z = pV->z / s;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>&
Vector3<T>::operator+=(const Self& v)
{
    this->setAdd(*this, v);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator-=(const Self& v)
{
    this->setSub(*this, v);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator*=(const Self& v)
{
    this->setMul(*this, v);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator/=(const Self& v)
{
    this->setDiv(*this, v);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator*=(T s)
{
    this->setScale(*this, s);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator/=(T s)
{
    this->setScaleInv(*this, s);
    return *this;
}

template <typename T>
inline void
Vector3<T>::set(T x_, T y_, T z_)
{
    this->x = x_;
    this->y = y_;
    this->z = z_;
}

template <typename T>
inline f32
Vector3<T>::length() const
{
    return Mathf::sqrt(squaredLength());
}

template <typename T>
inline T
Vector3<T>::dot(const Self& v) const
{
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

#if RIO_IS_CAFE

template <>
inline f32
Vector3<f32>::dot(const Vector3f& v) const
{
    const Vector3f* const pSelf = this;
    const Vector3f* const pV    = &v;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Load 2 floats from pV into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pV]), 0, 0" : [v1] "=f"(v1) : [pV] "r"(pV));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Add ps1(v0) to ps0(v0)
    asm volatile ("ps_sum0 %[v0], %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    // Handle z component
    v0 += pSelf->z * pV->z;

    return v0;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::cross(const Self& b) const
{
    Self o;
    o.setCross(*this, b);
    return o;
}

template <typename T>
inline void
Vector3<T>::setCross(const Self& a, const Self& b)
{
    RIO_ASSERT(this != &a && this != &b);

    this->x = a.y * b.z - a.z * b.y;
    this->y = a.z * b.x - a.x * b.z;
    this->z = a.x * b.y - a.y * b.x;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setCross(const Vector3f& a, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;
    f32 v2;
    f32 v3;
    f32 v4;
    f32 v5;
    f32 v6;
    f32 v7;
    f32 v8;
    f32 v9;
    f32 v10;

    asm volatile ("psq_l      %[v0],  0(%[pA]), 0, 0     " : [v0 ] "=f"(v0 ) : [pA] "r"(pA));
    asm volatile ("psq_l      %[v1],  0(%[pB]), 0, 0     " : [v1 ] "=f"(v1 ) : [pB] "r"(pB));
    asm volatile ("lfs        %[v2],  8(%[pA])           " : [v2 ] "=f"(v2 ) : [pA] "r"(pA));
    asm volatile ("lfs        %[v3],  8(%[pB])           " : [v3 ] "=f"(v3 ) : [pB] "r"(pB));
    asm volatile ("ps_mul     %[v4],  %[v1], %[v2]       " : [v4 ] "=f"(v4 ) : [v1] "f"(v1), [v2] "f"(v2));
    asm volatile ("ps_msub    %[v5],  %[v0], %[v3], %[v4]" : [v5 ] "=f"(v5 ) : [v0] "f"(v0), [v3] "f"(v3), [v4] "f"(v4));
    asm volatile ("ps_merge10 %[v6],  %[v1], %[v1]       " : [v6 ] "=f"(v6 ) : [v1] "f"(v1));
    asm volatile ("ps_muls0   %[v7],  %[v1], %[v0]       " : [v7 ] "=f"(v7 ) : [v1] "f"(v1), [v0] "f"(v0));
    asm volatile ("ps_msub    %[v8],  %[v0], %[v6], %[v7]" : [v8 ] "=f"(v8 ) : [v0] "f"(v0), [v6] "f"(v6), [v7] "f"(v7));
    asm volatile ("ps_merge11 %[v9],  %[v5], %[v5]       " : [v9 ] "=f"(v9 ) : [v5] "f"(v5));
    asm volatile ("ps_merge01 %[v10], %[v5], %[v8]       " : [v10] "=f"(v10) : [v5] "f"(v5), [v8] "f"(v8));
    asm volatile ("ps_neg     %[v10], %[v10]             " : [v10] "+f"(v10) : );

    asm volatile ("psq_st     %[v9],  0(%[pSelf]), 1, 0  " : : [v9 ] "f"(v9 ), [pSelf] "r"(pSelf) : "memory");
    asm volatile ("psq_st     %[v10], 4(%[pSelf]), 0, 0  " : : [v10] "f"(v10), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0 || this->z != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return {
        this->x * inv_length,
        this->y * inv_length,
        this->z * inv_length
    };
}

template <typename T>
inline void
Vector3<T>::setNormalized(const Self& v)
{
    RIO_ASSERT(v.x != 0 || v.y != 0 || v.z != 0);

    f32 inv_length = Mathf::rsqrt(v.squaredLength());

    this->x = v.x * inv_length;
    this->y = v.y * inv_length;
    this->z = v.z * inv_length;
}

template <typename T>
inline Vector3<T>
Vector3<T>::multAdd(const Self& c, const Self& b) const
{
    Self o;
    o.setMultAdd(*this, c, b);
    return o;
}

template <typename T>
inline void
Vector3<T>::setMultAdd(const Self& a, const Self& c, const Self& b)
{
    this->x = a.x * c.x + b.x;
    this->y = a.y * c.y + b.y;
    this->z = a.z * c.z + b.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setMultAdd(const Vector3f& a, const Vector3f& c, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pC    = &c;
    const Vector3f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;
    f32 v2;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pC into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pC]), 0, 0" : [v1] "=f"(v1) : [pC] "r"(pC));

    // Load 2 floats from pB into ps(v2)
    asm volatile ("psq_l %[v2], 0(%[pB]), 0, 0" : [v2] "=f"(v2) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1) and add ps(v2)
    asm volatile ("ps_madd %[v0], %[v0], %[v1], %[v2]" : [v0] "+f"(v0) : [v1] "f"(v1), [v2] "f"(v2));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Handle z component
    pSelf->z = pA->z * pC->z + pB->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::scaleAdd(T s, const Self& b) const
{
    Self o;
    o.setScaleAdd(*this, s, b);
    return o;
}

template <typename T>
inline void
Vector3<T>::setScaleAdd(const Self& a, T s, const Self& b)
{
    this->x = a.x * s + b.x;
    this->y = a.y * s + b.y;
    this->z = a.z * s + b.z;
}

#if RIO_IS_CAFE

template <>
inline void
Vector3<f32>::setScaleAdd(const Vector3f& a, f32 s, const Vector3f& b)
{
    Vector3f*       const pSelf = this;
    const Vector3f* const pA    = &a;
    const Vector3f* const pB    = &b;

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

    // Handle z component
    pSelf->z = pA->z * s + pB->z;
}

#endif // RIO_IS_CAFE

template <typename T>
inline bool
Vector4<T>::operator==(const Self& v) const
{
    return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
}

template <typename T>
inline Vector4<T>
Vector4<T>::operator-() const
{
    Self o;
    o.setOppositeDir(*this);
    return o;
}

template <typename T>
inline void
Vector4<T>::setOppositeDir(const Self& v)
{
    this->x = -v.x;
    this->y = -v.y;
    this->z = -v.z;
    this->w = -v.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setOppositeDir(const Vector4f& v)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pV    = &v;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Negate ps(v0)
    asm volatile ("ps_neg %[v0], %[v0]" : [v0] "+f"(v0));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pV+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Negate ps(v0)
    asm volatile ("ps_neg %[v0], %[v0]" : [v0] "+f"(v0));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::operator+(const Self& v) const
{
    Self o;
    o.setAdd(*this, v);
    return o;
}

template <typename T>
inline void
Vector4<T>::setAdd(const Self& a, const Self& b)
{
    this->x = a.x + b.x;
    this->y = a.y + b.y;
    this->z = a.z + b.z;
    this->w = a.w + b.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setAdd(const Vector4f& a, const Vector4f& b)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pA    = &a;
    const Vector4f* const pB    = &b;

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
inline Vector4<T>
Vector4<T>::operator-(const Self& v) const
{
    Self o;
    o.setSub(*this, v);
    return o;
}

template <typename T>
inline void
Vector4<T>::setSub(const Self& a, const Self& b)
{
    this->x = a.x - b.x;
    this->y = a.y - b.y;
    this->z = a.z - b.z;
    this->w = a.w - b.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setSub(const Vector4f& a, const Vector4f& b)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pA    = &a;
    const Vector4f* const pB    = &b;

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
inline Vector4<T>
Vector4<T>::operator*(const Self& v) const
{
    Self o;
    o.setMul(*this, v);
    return o;
}

template <typename T>
inline void
Vector4<T>::setMul(const Self& a, const Self& b)
{
    this->x = a.x * b.x;
    this->y = a.y * b.y;
    this->z = a.z * b.z;
    this->w = a.w * b.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setMul(const Vector4f& a, const Vector4f& b)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pA    = &a;
    const Vector4f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pA+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::operator/(const Self& v) const
{
    Self o;
    o.setDiv(*this, v);
    return o;
}

template <typename T>
inline void
Vector4<T>::setDiv(const Self& a, const Self& b)
{
    this->x = a.x / b.x;
    this->y = a.y / b.y;
    this->z = a.z / b.z;
    this->w = a.w / b.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setDiv(const Vector4f& a, const Vector4f& b)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pA    = &a;
    const Vector4f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pA+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pB+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pB]), 0, 0" : [v1] "=f"(v1) : [pB] "r"(pB));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::operator*(T s) const
{
    Self o;
    o.setScale(*this, s);
    return o;
}

template <typename T>
inline void
Vector4<T>::setScale(const Self& v, T s)
{
    this->x = v.x * s;
    this->y = v.y * s;
    this->z = v.z * s;
    this->w = v.w * s;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setScale(const Vector4f& v, f32 s)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pV    = &v;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Multiply ps(v0) by s
    asm volatile ("ps_muls0 %[v0], %[v0], %[s]" : [v0] "+f"(v0) : [s] "f"(s));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pV+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Multiply ps(v0) by s
    asm volatile ("ps_muls0 %[v0], %[v0], %[s]" : [v0] "+f"(v0) : [s] "f"(s));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::operator/(T s) const
{
    Self o;
    o.setScaleInv(*this, s);
    return o;
}

template <typename T>
inline void
Vector4<T>::setScaleInv(const Self& v, T s)
{
    this->x = v.x / s;
    this->y = v.y / s;
    this->z = v.z / s;
    this->w = v.w / s;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setScaleInv(const Vector4f& v, f32 s)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pV    = &v;

    // Temporary variables
    f32 v0;
    f32 v1;

    // Load 2 floats from pV into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Load s into ps(v1)
    asm volatile ("ps_merge00 %[v1], %[s], %[s]" : [v1] "=f"(v1) : [s] "f"(s));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pV+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pV]), 0, 0" : [v0] "=f"(v0) : [pV] "r"(pV));

    // Divide ps(v0) by ps(v1)
    asm volatile ("ps_div %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>&
Vector4<T>::operator+=(const Self& v)
{
    this->setAdd(*this, v);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator-=(const Self& v)
{
    this->setSub(*this, v);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator*=(const Self& v)
{
    this->setMul(*this, v);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator/=(const Self& v)
{
    this->setDiv(*this, v);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator*=(T s)
{
    this->setScale(*this, s);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator/=(T s)
{
    this->setScaleInv(*this, s);
    return *this;
}

template <typename T>
inline void
Vector4<T>::set(T x_, T y_, T z_, T w_)
{
    this->x = x_;
    this->y = y_;
    this->z = z_;
    this->w = w_;
}

template <typename T>
inline f32
Vector4<T>::length() const
{
    return Mathf::sqrt(squaredLength());
}

template <typename T>
inline T
Vector4<T>::dot(const Self& v) const
{
    return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}

#if RIO_IS_CAFE

template <>
inline f32
Vector4<f32>::dot(const Vector4f& v) const
{
    const Vector4f* const pSelf = this;
    const Vector4f* const pV    = &v;

    // Temporary variables
    f32 v0;
    f32 v1;
    f32 v2;
    f32 v3;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Load 2 floats from pV into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pV]), 0, 0" : [v1] "=f"(v1) : [pV] "r"(pV));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Add ps1(v0) to ps0(v0) and store result in ps(v2)
    asm volatile ("ps_sum0 %[v2], %[v0], %[v0], %[v0]" : [v2] "=f"(v2) : [v0] "f"(v0));

    // Load 2 floats from pSelf+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Load 2 floats from pV+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pV]), 0, 0" : [v1] "=f"(v1) : [pV] "r"(pV));

    // Multiply ps(v0) by ps(v1)
    asm volatile ("ps_mul %[v0], %[v0], %[v1]" : [v0] "+f"(v0) : [v1] "f"(v1));

    // Add ps1(v0) to ps0(v0) and store result in ps(v3)
    asm volatile ("ps_sum0 %[v3], %[v0], %[v0], %[v0]" : [v3] "=f"(v3) : [v0] "f"(v0));

    return v2 + v3;
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0 || this->z != 0 || this->w != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return {
        this->x * inv_length,
        this->y * inv_length,
        this->z * inv_length,
        this->w * inv_length
    };
}

template <typename T>
inline void
Vector4<T>::setNormalized(const Self& v)
{
    RIO_ASSERT(v.x != 0 || v.y != 0 || v.z != 0 || v.w != 0);

    f32 inv_length = Mathf::rsqrt(v.squaredLength());

    this->x = v.x * inv_length;
    this->y = v.y * inv_length;
    this->z = v.z * inv_length;
    this->w = v.w * inv_length;
}

template <typename T>
inline Vector4<T>
Vector4<T>::multAdd(const Self& c, const Self& b) const
{
    Self o;
    o.setMultAdd(*this, c, b);
    return o;
}

template <typename T>
inline void
Vector4<T>::setMultAdd(const Self& a, const Self& c, const Self& b)
{
    this->x = a.x * c.x + b.x;
    this->y = a.y * c.y + b.y;
    this->z = a.z * c.z + b.z;
    this->w = a.w * c.w + b.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setMultAdd(const Vector4f& a, const Vector4f& c, const Vector4f& b)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pA    = &a;
    const Vector4f* const pC    = &c;
    const Vector4f* const pB    = &b;

    // Temporary variables
    f32 v0;
    f32 v1;
    f32 v2;

    // Load 2 floats from pA into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pC into ps(v1)
    asm volatile ("psq_l %[v1], 0(%[pC]), 0, 0" : [v1] "=f"(v1) : [pC] "r"(pC));

    // Load 2 floats from pB into ps(v2)
    asm volatile ("psq_l %[v2], 0(%[pB]), 0, 0" : [v2] "=f"(v2) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1) and add ps(v2)
    asm volatile ("ps_madd %[v0], %[v0], %[v1], %[v2]" : [v0] "+f"(v0) : [v1] "f"(v1), [v2] "f"(v2));

    // Store ps(v0) as 2 floats in pSelf
    asm volatile ("psq_st %[v0], 0(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");

    // Load 2 floats from pA+8 into ps(v0)
    asm volatile ("psq_l %[v0], 8(%[pA]), 0, 0" : [v0] "=f"(v0) : [pA] "r"(pA));

    // Load 2 floats from pC+8 into ps(v1)
    asm volatile ("psq_l %[v1], 8(%[pC]), 0, 0" : [v1] "=f"(v1) : [pC] "r"(pC));

    // Load 2 floats from pB+8 into ps(v2)
    asm volatile ("psq_l %[v2], 8(%[pB]), 0, 0" : [v2] "=f"(v2) : [pB] "r"(pB));

    // Multiply ps(v0) by ps(v1) and add ps(v2)
    asm volatile ("ps_madd %[v0], %[v0], %[v1], %[v2]" : [v0] "+f"(v0) : [v1] "f"(v1), [v2] "f"(v2));

    // Store ps(v0) as 2 floats in pSelf+8
    asm volatile ("psq_st %[v0], 8(%[pSelf]), 0, 0" : : [v0] "f"(v0), [pSelf] "r"(pSelf) : "memory");
}

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::scaleAdd(T s, const Self& b) const
{
    Self o;
    o.setScaleAdd(*this, s, b);
    return o;
}

template <typename T>
inline void
Vector4<T>::setScaleAdd(const Self& a, T s, const Self& b)
{
    this->x = a.x * s + b.x;
    this->y = a.y * s + b.y;
    this->z = a.z * s + b.z;
    this->w = a.w * s + b.w;
}

#if RIO_IS_CAFE

template <>
inline void
Vector4<f32>::setScaleAdd(const Vector4f& a, f32 s, const Vector4f& b)
{
    Vector4f*       const pSelf = this;
    const Vector4f* const pA    = &a;
    const Vector4f* const pB    = &b;

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

}

#endif // RIO_MATH_VECTOR_IMPL_H
