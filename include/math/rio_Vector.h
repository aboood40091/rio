#ifndef RIO_MATH_VECTOR_H
#define RIO_MATH_VECTOR_H

#include <math/rio_MathTypes.h>

namespace rio {

template <typename T>
struct Vector2 : BaseVec2<T>
{
    typedef Vector2<T> Self;
    typedef T ValueType;

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const { return !operator==(v); }

    Self operator-() const;
    void setOppositeDir(const Self& v);

    Self operator+(const Self& v) const;
    void setAdd(const Self& a, const Self& b);

    Self operator-(const Self& v) const;
    void setSub(const Self& a, const Self& b);

    Self operator*(const Self& v) const;
    void setMul(const Self& a, const Self& b);

    Self operator/(const Self& v) const;
    void setDiv(const Self& a, const Self& b);

    Self operator*(T s) const;
    void setScale(const Self& v, T s);

    Self operator/(T s) const;
    void setScaleInv(const Self& v, T s);

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator*=(const Self& v);
    Self& operator/=(const Self& v);

    Self& operator*=(T s);
    Self& operator/=(T s);

    void set(T x_, T y_);

    T squaredLength() const { return dot(*this); }
    f32 length() const;

    T dot(const Self& v) const;

    f32 normalize() { return setNormalized(*this); }

    Self normalized() const;
    f32 setNormalized(const Self& v);

    Self multAdd(const Self& c, const Self& b) const;
    void setMultAdd(const Self& a, const Self& c, const Self& b);

    Self scaleAdd(T s, const Self& b) const;
    void setScaleAdd(const Self& a, T s, const Self& b);
};

typedef Vector2<s32> Vector2i;
typedef Vector2<s32> Vec2i;

typedef Vector2<u32> Vector2u;
typedef Vector2<u32> Vec2u;

typedef Vector2<f32> Vector2f;
typedef Vector2<f32> Vec2f;

template <typename T>
struct Vector3 : BaseVec3<T>
{
    typedef Vector3<T> Self;
    typedef T ValueType;

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const { return !operator==(v); }

    Self operator-() const;
    void setOppositeDir(const Self& v);

    Self operator+(const Self& v) const;
    void setAdd(const Self& a, const Self& b);

    Self operator-(const Self& v) const;
    void setSub(const Self& a, const Self& b);

    Self operator*(const Self& v) const;
    void setMul(const Self& a, const Self& b);

    Self operator/(const Self& v) const;
    void setDiv(const Self& a, const Self& b);

    Self operator*(T s) const;
    void setScale(const Self& v, T s);

    Self operator/(T s) const;
    void setScaleInv(const Self& v, T s);

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator*=(const Self& v);
    Self& operator/=(const Self& v);

    Self& operator*=(T s);
    Self& operator/=(T s);

    void set(T x_, T y_, T z_);

    T squaredLength() const { return dot(*this); }
    f32 length() const;

    T dot(const Self& v) const;

    Self cross(const Self& b) const;
    void setCross(const Self& a, const Self& b);

    f32 normalize() { return setNormalized(*this); }

    Self normalized() const;
    f32 setNormalized(const Self& v);

    Self multAdd(const Self& c, const Self& b) const;
    void setMultAdd(const Self& a, const Self& c, const Self& b);

    Self scaleAdd(T s, const Self& b) const;
    void setScaleAdd(const Self& a, T s, const Self& b);
};

typedef Vector3<s32> Vector3i;
typedef Vector3<s32> Vec3i;

typedef Vector3<u32> Vector3u;
typedef Vector3<u32> Vec3u;

typedef Vector3<f32> Vector3f;
typedef Vector3<f32> Vec3f;

template <typename T>
struct Vector4 : BaseVec4<T>
{
    typedef Vector4<T> Self;
    typedef T ValueType;

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const { return !operator==(v); }

    Self operator-() const;
    void setOppositeDir(const Self& v);

    Self operator+(const Self& v) const;
    void setAdd(const Self& a, const Self& b);

    Self operator-(const Self& v) const;
    void setSub(const Self& a, const Self& b);

    Self operator*(const Self& v) const;
    void setMul(const Self& a, const Self& b);

    Self operator/(const Self& v) const;
    void setDiv(const Self& a, const Self& b);

    Self operator*(T s) const;
    void setScale(const Self& v, T s);

    Self operator/(T s) const;
    void setScaleInv(const Self& v, T s);

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator*=(const Self& v);
    Self& operator/=(const Self& v);

    Self& operator*=(T s);
    Self& operator/=(T s);

    void set(T x_, T y_, T z_, T w_);

    T squaredLength() const { return dot(*this); }
    f32 length() const;

    T dot(const Self& v) const;

    f32 normalize() { return setNormalized(*this); }

    Self normalized() const;
    f32 setNormalized(const Self& v);

    Self multAdd(const Self& c, const Self& b) const;
    void setMultAdd(const Self& a, const Self& c, const Self& b);

    Self scaleAdd(T s, const Self& b) const;
    void setScaleAdd(const Self& a, T s, const Self& b);
};

typedef Vector4<s32> Vector4i;
typedef Vector4<s32> Vec4i;

typedef Vector4<u32> Vector4u;
typedef Vector4<u32> Vec4u;

typedef Vector4<f32> Vector4f;
typedef Vector4<f32> Vec4f;

}

#include <math/impl/rio_VectorImpl.h>

#endif // RIO_MATH_VECTOR_H
