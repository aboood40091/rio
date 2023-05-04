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
void
Vector2<f32>::setOppositeDir(const Vector2f& v);

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
void
Vector2<f32>::setAdd(const Vector2f& a, const Vector2f& b);

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
void
Vector2<f32>::setSub(const Vector2f& a, const Vector2f& b);

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
void
Vector2<f32>::setMul(const Vector2f& a, const Vector2f& b);

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
void
Vector2<f32>::setDiv(const Vector2f& a, const Vector2f& b);

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
void
Vector2<f32>::setScale(const Vector2f& v, f32 s);

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
void
Vector2<f32>::setScaleInv(const Vector2f& v, f32 s);

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

#if RIO_IS_CAFE

template <>
f32
Vector2<f32>::squaredLength() const;

#endif // RIO_IS_CAFE

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
f32
Vector2<f32>::dot(const Vector2f& v) const;

#endif // RIO_IS_CAFE

template <typename T>
inline Vector2<T>
Vector2<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return (*this) * inv_length;
}

template <typename T>
inline f32
Vector2<T>::setNormalized(const Self& v)
{
    RIO_ASSERT(v.x != 0 || v.y != 0);

    f32 sq_length = v.squaredLength();
    f32 inv_length = Mathf::rsqrt(sq_length);

    setScale(v, inv_length);

    return sq_length * inv_length;
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
void
Vector2<f32>::setMultAdd(const Vector2f& a, const Vector2f& c, const Vector2f& b);

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
void
Vector2<f32>::setScaleAdd(const Vector2f& a, f32 s, const Vector2f& b);

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
void
Vector3<f32>::setOppositeDir(const Vector3f& v);

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
void
Vector3<f32>::setAdd(const Vector3f& a, const Vector3f& b);

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
void
Vector3<f32>::setSub(const Vector3f& a, const Vector3f& b);

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
void
Vector3<f32>::setMul(const Vector3f& a, const Vector3f& b);

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
void
Vector3<f32>::setDiv(const Vector3f& a, const Vector3f& b);

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
void
Vector3<f32>::setScale(const Vector3f& v, f32 s);

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
void
Vector3<f32>::setScaleInv(const Vector3f& v, f32 s);

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

#if RIO_IS_CAFE

template <>
f32
Vector3<f32>::squaredLength() const;

#endif // RIO_IS_CAFE

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
f32
Vector3<f32>::dot(const Vector3f& v) const;

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
void
Vector3<f32>::setCross(const Vector3f& a, const Vector3f& b);

#endif // RIO_IS_CAFE

template <typename T>
inline Vector3<T>
Vector3<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0 || this->z != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return (*this) * inv_length;
}

template <typename T>
inline f32
Vector3<T>::setNormalized(const Self& v)
{
    RIO_ASSERT(v.x != 0 || v.y != 0 || v.z != 0);

    f32 sq_length = v.squaredLength();
    f32 inv_length = Mathf::rsqrt(sq_length);

    setScale(v, inv_length);

    return sq_length * inv_length;
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
void
Vector3<f32>::setMultAdd(const Vector3f& a, const Vector3f& c, const Vector3f& b);

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
void
Vector3<f32>::setScaleAdd(const Vector3f& a, f32 s, const Vector3f& b);

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
void
Vector4<f32>::setOppositeDir(const Vector4f& v);

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
void
Vector4<f32>::setAdd(const Vector4f& a, const Vector4f& b);

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
void
Vector4<f32>::setSub(const Vector4f& a, const Vector4f& b);

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
void
Vector4<f32>::setMul(const Vector4f& a, const Vector4f& b);

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
void
Vector4<f32>::setDiv(const Vector4f& a, const Vector4f& b);

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
void
Vector4<f32>::setScale(const Vector4f& v, f32 s);

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
void
Vector4<f32>::setScaleInv(const Vector4f& v, f32 s);

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

#if RIO_IS_CAFE

template <>
f32
Vector4<f32>::squaredLength() const;

#endif // RIO_IS_CAFE

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
f32
Vector4<f32>::dot(const Vector4f& v) const;

#endif // RIO_IS_CAFE

template <typename T>
inline Vector4<T>
Vector4<T>::normalized() const
{
    RIO_ASSERT(this->x != 0 || this->y != 0 || this->z != 0 || this->w != 0);

    f32 inv_length = Mathf::rsqrt(squaredLength());

    return (*this) * inv_length;
}

template <typename T>
inline f32
Vector4<T>::setNormalized(const Self& v)
{
    RIO_ASSERT(v.x != 0 || v.y != 0 || v.z != 0 || v.w != 0);

    f32 sq_length = v.squaredLength();
    f32 inv_length = Mathf::rsqrt(sq_length);

    setScale(v, inv_length);

    return sq_length * inv_length;
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
void
Vector4<f32>::setMultAdd(const Vector4f& a, const Vector4f& c, const Vector4f& b);

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
void
Vector4<f32>::setScaleAdd(const Vector4f& a, f32 s, const Vector4f& b);

#endif // RIO_IS_CAFE

}

#endif // RIO_MATH_VECTOR_IMPL_H
