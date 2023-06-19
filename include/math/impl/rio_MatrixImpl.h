#ifndef RIO_MATH_MATRIX_IMPL_H
#define RIO_MATH_MATRIX_IMPL_H

// This file is already included in rio_Matrix.h
//#include <math/rio_Matrix.h>

#include <math/rio_Math.h>
#include <math/rio_Vector.h>

// TODO: PS implementation for Cafe

namespace rio {

template <typename T>
inline bool
Matrix34<T>::operator==(const Self& n) const
{
    return this->v[0] == n.v[0] &&
           this->v[1] == n.v[1] &&
           this->v[2] == n.v[2];
}

template <typename T>
inline Matrix34<T>
Matrix34<T>::operator+(const Self& n) const
{
    Self o;
    o.v[0].setAdd(this->v[0], n.v[0]);
    o.v[1].setAdd(this->v[1], n.v[1]);
    o.v[2].setAdd(this->v[2], n.v[2]);
    return o;
}

template <typename T>
inline Matrix34<T>
Matrix34<T>::operator-(const Self& n) const
{
    Self o;
    o.v[0].setSub(this->v[0], n.v[0]);
    o.v[1].setSub(this->v[1], n.v[1]);
    o.v[2].setSub(this->v[2], n.v[2]);
    return o;
}

template <typename T>
inline Matrix34<T>
Matrix34<T>::operator*(T s) const
{
    Self o;
    o.v[0].setScale(this->v[0], s);
    o.v[1].setScale(this->v[1], s);
    o.v[2].setScale(this->v[2], s);
    return o;
}

template <typename T>
inline Matrix34<T>
Matrix34<T>::operator/(T s) const
{
    Self o;
    o.v[0].setScaleInv(this->v[0], s);
    o.v[1].setScaleInv(this->v[1], s);
    o.v[2].setScaleInv(this->v[2], s);
    return o;
}

template <typename T>
inline Matrix34<T>&
Matrix34<T>::operator+=(const Self& n)
{
    this->v[0] += n.v[0];
    this->v[1] += n.v[1];
    this->v[2] += n.v[2];
    return *this;
}

template <typename T>
inline Matrix34<T>&
Matrix34<T>::operator-=(const Self& n)
{
    this->v[0] -= n.v[0];
    this->v[1] -= n.v[1];
    this->v[2] -= n.v[2];
    return *this;
}

template <typename T>
inline Matrix34<T>&
Matrix34<T>::operator*=(T s)
{
    this->v[0] *= s;
    this->v[1] *= s;
    this->v[2] *= s;
    return *this;
}

template <typename T>
inline Matrix34<T>&
Matrix34<T>::operator/=(T s)
{
    this->v[0] /= s;
    this->v[1] /= s;
    this->v[2] /= s;
    return *this;
}

template <typename T>
inline bool
Matrix34<T>::setInverse(const Self& n)
{
    const T a11 = n.m[0][0];
    const T a12 = n.m[0][1];
    const T a13 = n.m[0][2];
    const T a14 = n.m[0][3];

    const T a21 = n.m[1][0];
    const T a22 = n.m[1][1];
    const T a23 = n.m[1][2];
    const T a24 = n.m[1][3];

    const T a31 = n.m[2][0];
    const T a32 = n.m[2][1];
    const T a33 = n.m[2][2];
    const T a34 = n.m[2][3];

    T det = (a11 * a22 * a33 - a31 * a22 * a13)
          + (a12 * a23 * a31 - a21 * a12 * a33)
          + (a13 * a21 * a32 - a11 * a32 * a23);

    if (det == 0)
        return false;

    det = 1 / det;

    this->m[0][0] = (a22 * a33 - a32 * a23) * det;
    this->m[0][1] = (a32 * a13 - a12 * a33) * det;
    this->m[0][2] = (a12 * a23 - a22 * a13) * det;

    this->m[1][0] = (a31 * a23 - a21 * a33) * det;
    this->m[1][1] = (a11 * a33 - a31 * a13) * det;
    this->m[1][2] = (a21 * a13 - a11 * a23) * det;

    this->m[2][0] = (a21 * a32 - a31 * a22) * det;
    this->m[2][1] = (a31 * a12 - a11 * a32) * det;
    this->m[2][2] = (a11 * a22 - a21 * a12) * det;

    this->m[0][3] = this->m[0][0] * -a14 + this->m[0][1] * -a24 + this->m[0][2] * -a34;
    this->m[1][3] = this->m[1][0] * -a14 + this->m[1][1] * -a24 + this->m[1][2] * -a34;
    this->m[2][3] = this->m[2][0] * -a14 + this->m[2][1] * -a24 + this->m[2][2] * -a34;

    return true;
}

template <typename T>
inline bool
Matrix34<T>::setInverseTranspose(const Self& n)
{
    const T a11 = n.m[0][0];
    const T a12 = n.m[0][1];
    const T a13 = n.m[0][2];

    const T a21 = n.m[1][0];
    const T a22 = n.m[1][1];
    const T a23 = n.m[1][2];

    const T a31 = n.m[2][0];
    const T a32 = n.m[2][1];
    const T a33 = n.m[2][2];

    T det = (a11 * a22 * a33 - a31 * a22 * a13)
          + (a12 * a23 * a31 - a21 * a12 * a33)
          + (a13 * a21 * a32 - a11 * a32 * a23);

    if (det == 0)
        return false;

    det = 1 / det;

    this->m[0][0] = (a22 * a33 - a23 * a32) * det;
    this->m[0][1] = (a23 * a31 - a21 * a33) * det;
    this->m[0][2] = (a21 * a32 - a22 * a31) * det;

    this->m[1][0] = (a13 * a32 - a12 * a33) * det;
    this->m[1][1] = (a11 * a33 - a13 * a31) * det;
    this->m[1][2] = (a12 * a31 - a11 * a32) * det;

    this->m[2][0] = (a12 * a23 - a13 * a22) * det;
    this->m[2][1] = (a13 * a21 - a11 * a23) * det;
    this->m[2][2] = (a11 * a22 - a12 * a21) * det;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;

    return true;
}

template <typename T>
inline void
Matrix34<T>::setMul(const Self& a, const Self& b)
{
    const T a11 = a.m[0][0];
    const T a12 = a.m[0][1];
    const T a13 = a.m[0][2];
    const T a14 = a.m[0][3];

    const T a21 = a.m[1][0];
    const T a22 = a.m[1][1];
    const T a23 = a.m[1][2];
    const T a24 = a.m[1][3];

    const T a31 = a.m[2][0];
    const T a32 = a.m[2][1];
    const T a33 = a.m[2][2];
    const T a34 = a.m[2][3];

    const T b11 = b.m[0][0];
    const T b12 = b.m[0][1];
    const T b13 = b.m[0][2];
    const T b14 = b.m[0][3];

    const T b21 = b.m[1][0];
    const T b22 = b.m[1][1];
    const T b23 = b.m[1][2];
    const T b24 = b.m[1][3];

    const T b31 = b.m[2][0];
    const T b32 = b.m[2][1];
    const T b33 = b.m[2][2];
    const T b34 = b.m[2][3];

    this->m[0][0] = a11 * b11 + a12 * b21 + a13 * b31;
    this->m[0][1] = a11 * b12 + a12 * b22 + a13 * b32;
    this->m[0][2] = a11 * b13 + a12 * b23 + a13 * b33;
    this->m[0][3] = a11 * b14 + a12 * b24 + a13 * b34 + a14;

    this->m[1][0] = a21 * b11 + a22 * b21 + a23 * b31;
    this->m[1][1] = a21 * b12 + a22 * b22 + a23 * b32;
    this->m[1][2] = a21 * b13 + a22 * b23 + a23 * b33;
    this->m[1][3] = a21 * b14 + a22 * b24 + a23 * b34 + a24;

    this->m[2][0] = a31 * b11 + a32 * b21 + a33 * b31;
    this->m[2][1] = a31 * b12 + a32 * b22 + a33 * b32;
    this->m[2][2] = a31 * b13 + a32 * b23 + a33 * b33;
    this->m[2][3] = a31 * b14 + a32 * b24 + a33 * b34 + a34;
}

template <typename T>
inline void
Matrix34<T>::setTranspose(const Self& n)
{
    RIO_ASSERT(this != &n);

    this->m[0][0] = n.m[0][0];
    this->m[0][1] = n.m[1][0];
    this->m[0][2] = n.m[2][0];
    this->m[0][3] = 0;

    this->m[1][0] = n.m[0][1];
    this->m[1][1] = n.m[1][1];
    this->m[1][2] = n.m[2][1];
    this->m[1][3] = 0;

    this->m[2][0] = n.m[0][2];
    this->m[2][1] = n.m[1][2];
    this->m[2][2] = n.m[2][2];
    this->m[2][3] = 0;
}

template <typename T>
inline void
Matrix34<T>::transpose()
{
    const T a12 = this->m[0][1];
    const T a13 = this->m[0][2];

    const T a21 = this->m[1][0];
    const T a23 = this->m[1][2];

    const T a31 = this->m[2][0];
    const T a32 = this->m[2][1];

    this->m[0][1] = a21;
    this->m[0][2] = a31;
    this->m[0][3] = 0;

    this->m[1][0] = a12;
    this->m[1][2] = a32;
    this->m[1][3] = 0;

    this->m[2][0] = a13;
    this->m[2][1] = a23;
    this->m[2][3] = 0;
}

template <typename T>
inline void
Matrix34<T>::makeS(const Vec3& s)
{
    this->m[0][0] = s.x;
    this->m[1][0] = 0;
    this->m[2][0] = 0;

    this->m[0][1] = 0;
    this->m[1][1] = s.y;
    this->m[2][1] = 0;

    this->m[0][2] = 0;
    this->m[1][2] = 0;
    this->m[2][2] = s.z;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
}

template <typename T>
inline void
Matrix34<T>::makeR(const Vec3& r)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = (cosV[1] * cosV[2]);
    this->m[1][0] = (cosV[1] * sinV[2]);
    this->m[2][0] = -sinV[1];

    this->m[0][1] = (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = (sinV[0] * cosV[1]);

    this->m[0][2] = (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = (cosV[0] * cosV[1]);

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
}

template <typename T>
void Matrix34<T>::makeQ(const Quat& q)
{
    // Assuming the quaternion "q" is normalized

    const T yy = 2 * q.y * q.y;
    const T zz = 2 * q.z * q.z;
    const T xx = 2 * q.x * q.x;
    const T xy = 2 * q.x * q.y;
    const T xz = 2 * q.x * q.z;
    const T yz = 2 * q.y * q.z;
    const T wz = 2 * q.w * q.z;
    const T wx = 2 * q.w * q.x;
    const T wy = 2 * q.w * q.y;

    this->m[0][0] = 1 - yy - zz;
    this->m[0][1] =     xy - wz;
    this->m[0][2] =     xz + wy;

    this->m[1][0] =     xy + wz;
    this->m[1][1] = 1 - xx - zz;
    this->m[1][2] =     yz - wx;

    this->m[2][0] =     xz - wy;
    this->m[2][1] =     yz + wx;
    this->m[2][2] = 1 - xx - yy;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
}

template <typename T>
inline void
Matrix34<T>::makeT(const Vec3& t)
{
    this->m[0][0] = 1;
    this->m[1][0] = 0;
    this->m[2][0] = 0;

    this->m[0][1] = 0;
    this->m[1][1] = 1;
    this->m[2][1] = 0;

    this->m[0][2] = 0;
    this->m[1][2] = 0;
    this->m[2][2] = 1;

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix34<T>::makeSR(const Vec3& s, const Vec3& r)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = s.x * (cosV[1] * cosV[2]);
    this->m[1][0] = s.x * (cosV[1] * sinV[2]);
    this->m[2][0] = s.x * -sinV[1];

    this->m[0][1] = s.y * (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = s.y * (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = s.y * (sinV[0] * cosV[1]);

    this->m[0][2] = s.z * (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = s.z * (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = s.z * (cosV[0] * cosV[1]);

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
}

template <typename T>
inline void
Matrix34<T>::makeSQ(const Vec3& s, const Quat& q)
{
    makeQ(q);

    *(Vector3<T>*)(&this->m[0][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[1][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[2][0]) *= static_cast<const Vector3<T>&>(s);
}

template <typename T>
inline void
Matrix34<T>::makeST(const Vec3& s, const Vec3& t)
{
    this->m[0][0] = s.x;
    this->m[1][0] = 0;
    this->m[2][0] = 0;

    this->m[0][1] = 0;
    this->m[1][1] = s.y;
    this->m[2][1] = 0;

    this->m[0][2] = 0;
    this->m[1][2] = 0;
    this->m[2][2] = s.z;

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix34<T>::makeRT(const Vec3& r, const Vec3& t)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = (cosV[1] * cosV[2]);
    this->m[1][0] = (cosV[1] * sinV[2]);
    this->m[2][0] = -sinV[1];

    this->m[0][1] = (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = (sinV[0] * cosV[1]);

    this->m[0][2] = (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = (cosV[0] * cosV[1]);

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix34<T>::makeQT(const Quat& q, const Vec3& t)
{
    makeQ(q);

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix34<T>::makeSRT(const Vec3& s, const Vec3& r, const Vec3& t)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = s.x * (cosV[1] * cosV[2]);
    this->m[1][0] = s.x * (cosV[1] * sinV[2]);
    this->m[2][0] = s.x * -sinV[1];

    this->m[0][1] = s.y * (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = s.y * (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = s.y * (sinV[0] * cosV[1]);

    this->m[0][2] = s.z * (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = s.z * (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = s.z * (cosV[0] * cosV[1]);

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix34<T>::makeSQT(const Vec3& s, const Quat& q, const Vec3& t)
{
    makeQ(q);

    *(Vector3<T>*)(&this->m[0][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[1][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[2][0]) *= static_cast<const Vector3<T>&>(s);

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline bool
Matrix34<T>::makeVectorRotation(const Vec3& from, const Vec3& to)
{
    const Vector3<T>& from_ = static_cast<const Vector3<T>&>(from);
    const Vector3<T>& to_   = static_cast<const Vector3<T>&>(to);

    Vector3<T> cross;
    cross.setCross(from_, to_);
    const T dot = Mathf::abs(from_.dot(to_) + 1);

    if (dot <= std::numeric_limits<T>::epsilon())
        return false;

    const T v0 = 2 * dot;
    const T v2 = Math<T>::rsqrt(v0);
    const T v1 = v2 * v0;

    const T w = v1 * 0.5f;
    const T x = cross.x * v2;
    const T y = cross.y * v2;
    const T z = cross.z * v2;

    const T yy = 2 * y * y;
    const T zz = 2 * z * z;
    const T xx = 2 * x * x;
    const T xy = 2 * x * y;
    const T xz = 2 * x * z;
    const T yz = 2 * y * z;
    const T wz = 2 * w * z;
    const T wx = 2 * w * x;
    const T wy = 2 * w * y;

    this->m[0][0] = 1 - yy - zz;
    this->m[0][1] =     xy - wz;
    this->m[0][2] =     xz + wy;

    this->m[1][0] =     xy + wz;
    this->m[1][1] = 1 - xx - zz;
    this->m[1][2] =     yz - wx;

    this->m[2][0] =     xz - wy;
    this->m[2][1] =     yz + wx;
    this->m[2][2] = 1 - xx - yy;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;

    return true;
}

template <typename T>
inline void
Matrix34<T>::applyScaleLocal(T s)
{
    *(Vec3*)&(this->v[0]) *= s;
    *(Vec3*)&(this->v[1]) *= s;
    *(Vec3*)&(this->v[2]) *= s;
}

template <typename T>
inline void
Matrix34<T>::applyScaleLocal(const Vec3& s)
{
    this->m[0][0] *= s.x;
    this->m[1][0] *= s.x;
    this->m[2][0] *= s.x;

    this->m[0][1] *= s.y;
    this->m[1][1] *= s.y;
    this->m[2][1] *= s.y;

    this->m[0][2] *= s.z;
    this->m[1][2] *= s.z;
    this->m[2][2] *= s.z;
}

template <typename T>
inline void
Matrix34<T>::applyScaleWorld(T s)
{
    this->v[0] *= s;
    this->v[1] *= s;
    this->v[2] *= s;
}

template <typename T>
inline void
Matrix34<T>::applyScaleWorld(const Vec3& s)
{
    this->v[0] *= s.x;
    this->v[1] *= s.y;
    this->v[2] *= s.z;
}

template <typename T>
inline void
Matrix34<T>::applyTranslationLocal(const Vec3& t)
{
    this->m[0][3] += this->m[0][0] * t.x + this->m[0][1] * t.y + this->m[0][2] * t.z;
    this->m[1][3] += this->m[1][0] * t.x + this->m[1][1] * t.y + this->m[1][2] * t.z;
    this->m[2][3] += this->m[2][0] * t.x + this->m[2][1] * t.y + this->m[2][2] * t.z;
}

template <typename T>
inline void
Matrix34<T>::setTranslationWorld(const Vec3& t)
{
    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix34<T>::applyTranslationWorld(const Vec3& t)
{
    this->m[0][3] += t.x;
    this->m[1][3] += t.y;
    this->m[2][3] += t.z;
}

template <typename T>
inline bool
Matrix44<T>::operator==(const Self& n) const
{
    return this->v[0] == n.v[0] &&
           this->v[1] == n.v[1] &&
           this->v[2] == n.v[2] &&
           this->v[3] == n.v[3];
}

template <typename T>
inline Matrix44<T>
Matrix44<T>::operator+(const Self& n) const
{
    Self o;
    o.v[0].setAdd(this->v[0], n.v[0]);
    o.v[1].setAdd(this->v[1], n.v[1]);
    o.v[2].setAdd(this->v[2], n.v[2]);
    o.v[3].setAdd(this->v[3], n.v[3]);
    return o;
}

template <typename T>
inline Matrix44<T>
Matrix44<T>::operator-(const Self& n) const
{
    Self o;
    o.v[0].setSub(this->v[0], n.v[0]);
    o.v[1].setSub(this->v[1], n.v[1]);
    o.v[2].setSub(this->v[2], n.v[2]);
    o.v[3].setSub(this->v[3], n.v[3]);
    return o;
}

template <typename T>
inline Matrix44<T>
Matrix44<T>::operator*(T s) const
{
    Self o;
    o.v[0].setScale(this->v[0], s);
    o.v[1].setScale(this->v[1], s);
    o.v[2].setScale(this->v[2], s);
    o.v[3].setScale(this->v[3], s);
    return o;
}

template <typename T>
inline Matrix44<T>
Matrix44<T>::operator/(T s) const
{
    Self o;
    o.v[0].setScaleInv(this->v[0], s);
    o.v[1].setScaleInv(this->v[1], s);
    o.v[2].setScaleInv(this->v[2], s);
    o.v[3].setScaleInv(this->v[3], s);
    return o;
}

template <typename T>
inline Matrix44<T>&
Matrix44<T>::operator+=(const Self& n)
{
    this->v[0] += n.v[0];
    this->v[1] += n.v[1];
    this->v[2] += n.v[2];
    this->v[3] += n.v[3];
    return *this;
}

template <typename T>
inline Matrix44<T>&
Matrix44<T>::operator-=(const Self& n)
{
    this->v[0] -= n.v[0];
    this->v[1] -= n.v[1];
    this->v[2] -= n.v[2];
    this->v[3] -= n.v[3];
    return *this;
}

template <typename T>
inline Matrix44<T>&
Matrix44<T>::operator*=(T s)
{
    this->v[0] *= s;
    this->v[1] *= s;
    this->v[2] *= s;
    this->v[3] *= s;
    return *this;
}

template <typename T>
inline Matrix44<T>&
Matrix44<T>::operator/=(T s)
{
    this->v[0] /= s;
    this->v[1] /= s;
    this->v[2] /= s;
    this->v[3] /= s;
    return *this;
}

template <typename T>
inline bool
Matrix44<T>::setInverse(const Self& n)
{
    const T a11 = n.m[0][0];
    const T a12 = n.m[0][1];
    const T a13 = n.m[0][2];
    const T a14 = n.m[0][3];

    const T a21 = n.m[1][0];
    const T a22 = n.m[1][1];
    const T a23 = n.m[1][2];
    const T a24 = n.m[1][3];

    const T a31 = n.m[2][0];
    const T a32 = n.m[2][1];
    const T a33 = n.m[2][2];
    const T a34 = n.m[2][3];

    const T a41 = n.m[3][0];
    const T a42 = n.m[3][1];
    const T a43 = n.m[3][2];
    const T a44 = n.m[3][3];

    T det = a11 * (a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43)
          + a12 * (a21 * a34 * a43 + a23 * a31 * a44 + a24 * a33 * a41)
          + a13 * (a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42)
          + a14 * (a21 * a33 * a42 + a22 * a31 * a43 + a23 * a32 * a41)
          - a11 * (a22 * a34 * a43 + a23 * a32 * a44 + a24 * a33 * a42)
          - a12 * (a21 * a33 * a44 + a23 * a34 * a41 + a24 * a31 * a43)
          - a13 * (a21 * a34 * a42 + a22 * a31 * a44 + a24 * a32 * a41)
          - a14 * (a21 * a32 * a43 + a22 * a33 * a41 + a23 * a31 * a42);

    if (det == 0)
        return false;

    det = 1 / det;

    const T a33xa44_a34xa43 = a33 * a44 - a34 * a43;
    const T a32xa44_a34xa42 = a32 * a44 - a34 * a42;
    const T a33xa42_a32xa43 = a33 * a42 - a32 * a43;
    const T a33xa41_a31xa43 = a33 * a41 - a31 * a43;
    const T a31xa44_a34xa41 = a31 * a44 - a34 * a41;
    const T a32xa41_a31xa42 = a32 * a41 - a31 * a42;
    const T a23xa44_a24xa43 = a23 * a44 - a24 * a43;
    const T a24xa33_a23xa34 = a24 * a33 - a23 * a34;
    const T a24xa42_a22xa44 = a24 * a42 - a22 * a44;
    const T a22xa43_a23xa42 = a22 * a43 - a23 * a42;
    const T a22xa34_a24xa32 = a22 * a34 - a24 * a32;
    const T a23xa32_a22xa33 = a23 * a32 - a22 * a33;
    const T a21xa44_a24xa41 = a21 * a44 - a24 * a41;
    const T a23xa41_a21xa43 = a23 * a41 - a21 * a43;
    const T a24xa31_a21xa34 = a24 * a31 - a21 * a34;
    const T a21xa33_a23xa31 = a21 * a33 - a23 * a31;
    const T a21xa42_a22xa41 = a21 * a42 - a22 * a41;
    const T a22xa31_a21xa32 = a22 * a31 - a21 * a32;

    this->m[0][0] =  (a22 * a33xa44_a34xa43 - a23 * a32xa44_a34xa42 - a24 * a33xa42_a32xa43) * det;
    this->m[0][1] =  (a13 * a32xa44_a34xa42 + a14 * a33xa42_a32xa43 - a12 * a33xa44_a34xa43) * det;
    this->m[0][2] =  (a12 * a23xa44_a24xa43 + a13 * a24xa42_a22xa44 + a14 * a22xa43_a23xa42) * det;
    this->m[0][3] =  (a12 * a24xa33_a23xa34 + a13 * a22xa34_a24xa32 + a14 * a23xa32_a22xa33) * det;

    this->m[1][0] =  (a23 * a31xa44_a34xa41 + a24 * a33xa41_a31xa43 - a21 * a33xa44_a34xa43) * det;
    this->m[1][1] =  (a11 * a33xa44_a34xa43 - a13 * a31xa44_a34xa41 - a14 * a33xa41_a31xa43) * det;
    this->m[1][2] =  (a13 * a21xa44_a24xa41 + a14 * a23xa41_a21xa43 - a11 * a23xa44_a24xa43) * det;
    this->m[1][3] =  (a13 * a24xa31_a21xa34 + a14 * a21xa33_a23xa31 - a11 * a24xa33_a23xa34) * det;

    this->m[2][0] =  (a21 * a32xa44_a34xa42 - a22 * a31xa44_a34xa41 - a24 * a32xa41_a31xa42) * det;
    this->m[2][1] =  (a12 * a31xa44_a34xa41 + a14 * a32xa41_a31xa42 - a11 * a32xa44_a34xa42) * det;
    this->m[2][2] =  (a14 * a21xa42_a22xa41 - a11 * a24xa42_a22xa44 - a12 * a21xa44_a24xa41) * det;
    this->m[2][3] =  (a14 * a22xa31_a21xa32 - a11 * a22xa34_a24xa32 - a12 * a24xa31_a21xa34) * det;

    this->m[3][0] =  (a21 * a33xa42_a32xa43 - a22 * a33xa41_a31xa43 + a23 * a32xa41_a31xa42) * det;
    this->m[3][1] =  (a12 * a33xa41_a31xa43 - a13 * a32xa41_a31xa42 - a11 * a33xa42_a32xa43) * det;
    this->m[3][2] = (-a13 * a21xa42_a22xa41 - a11 * a22xa43_a23xa42 - a12 * a23xa41_a21xa43) * det;
    this->m[3][3] = (-a13 * a22xa31_a21xa32 - a11 * a23xa32_a22xa33 - a12 * a21xa33_a23xa31) * det;

    return true;
}

template <typename T>
inline void
Matrix44<T>::setMul(const Self& a, const Self& b)
{
    const T a11 = a.m[0][0];
    const T a12 = a.m[0][1];
    const T a13 = a.m[0][2];
    const T a14 = a.m[0][3];

    const T a21 = a.m[1][0];
    const T a22 = a.m[1][1];
    const T a23 = a.m[1][2];
    const T a24 = a.m[1][3];

    const T a31 = a.m[2][0];
    const T a32 = a.m[2][1];
    const T a33 = a.m[2][2];
    const T a34 = a.m[2][3];

    const T a41 = a.m[3][0];
    const T a42 = a.m[3][1];
    const T a43 = a.m[3][2];
    const T a44 = a.m[3][3];

    const T b11 = b.m[0][0];
    const T b12 = b.m[0][1];
    const T b13 = b.m[0][2];
    const T b14 = b.m[0][3];

    const T b21 = b.m[1][0];
    const T b22 = b.m[1][1];
    const T b23 = b.m[1][2];
    const T b24 = b.m[1][3];

    const T b31 = b.m[2][0];
    const T b32 = b.m[2][1];
    const T b33 = b.m[2][2];
    const T b34 = b.m[2][3];

    const T b41 = b.m[3][0];
    const T b42 = b.m[3][1];
    const T b43 = b.m[3][2];
    const T b44 = b.m[3][3];

    this->m[0][0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
    this->m[0][1] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
    this->m[0][2] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
    this->m[0][3] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

    this->m[1][0] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
    this->m[1][1] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
    this->m[1][2] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
    this->m[1][3] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

    this->m[2][0] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
    this->m[2][1] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
    this->m[2][2] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
    this->m[2][3] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

    this->m[3][0] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
    this->m[3][1] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
    this->m[3][2] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
    this->m[3][3] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;
}

template <typename T>
inline void
Matrix44<T>::setMul(const Mtx34& a, const Self& b)
{
    const T a11 = a.m[0][0];
    const T a12 = a.m[0][1];
    const T a13 = a.m[0][2];
    const T a14 = a.m[0][3];

    const T a21 = a.m[1][0];
    const T a22 = a.m[1][1];
    const T a23 = a.m[1][2];
    const T a24 = a.m[1][3];

    const T a31 = a.m[2][0];
    const T a32 = a.m[2][1];
    const T a33 = a.m[2][2];
    const T a34 = a.m[2][3];

    const T b11 = b.m[0][0];
    const T b12 = b.m[0][1];
    const T b13 = b.m[0][2];
    const T b14 = b.m[0][3];

    const T b21 = b.m[1][0];
    const T b22 = b.m[1][1];
    const T b23 = b.m[1][2];
    const T b24 = b.m[1][3];

    const T b31 = b.m[2][0];
    const T b32 = b.m[2][1];
    const T b33 = b.m[2][2];
    const T b34 = b.m[2][3];

    const T b41 = b.m[3][0];
    const T b42 = b.m[3][1];
    const T b43 = b.m[3][2];
    const T b44 = b.m[3][3];

    this->m[0][0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
    this->m[0][1] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
    this->m[0][2] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
    this->m[0][3] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

    this->m[1][0] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
    this->m[1][1] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
    this->m[1][2] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
    this->m[1][3] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

    this->m[2][0] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
    this->m[2][1] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
    this->m[2][2] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
    this->m[2][3] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

    this->m[3][0] = b41;
    this->m[3][1] = b42;
    this->m[3][2] = b43;
    this->m[3][3] = b44;
}

template <typename T>
inline void
Matrix44<T>::setMul(const Self& a, const Mtx34& b)
{
    const T a11 = a.m[0][0];
    const T a12 = a.m[0][1];
    const T a13 = a.m[0][2];
    const T a14 = a.m[0][3];

    const T a21 = a.m[1][0];
    const T a22 = a.m[1][1];
    const T a23 = a.m[1][2];
    const T a24 = a.m[1][3];

    const T a31 = a.m[2][0];
    const T a32 = a.m[2][1];
    const T a33 = a.m[2][2];
    const T a34 = a.m[2][3];

    const T a41 = a.m[3][0];
    const T a42 = a.m[3][1];
    const T a43 = a.m[3][2];
    const T a44 = a.m[3][3];

    const T b11 = b.m[0][0];
    const T b12 = b.m[0][1];
    const T b13 = b.m[0][2];
    const T b14 = b.m[0][3];

    const T b21 = b.m[1][0];
    const T b22 = b.m[1][1];
    const T b23 = b.m[1][2];
    const T b24 = b.m[1][3];

    const T b31 = b.m[2][0];
    const T b32 = b.m[2][1];
    const T b33 = b.m[2][2];
    const T b34 = b.m[2][3];

    this->m[0][0] = a11 * b11 + a12 * b21 + a13 * b31;
    this->m[0][1] = a11 * b12 + a12 * b22 + a13 * b32;
    this->m[0][2] = a11 * b13 + a12 * b23 + a13 * b33;
    this->m[0][3] = a11 * b14 + a12 * b24 + a13 * b34 + a14;

    this->m[1][0] = a21 * b11 + a22 * b21 + a23 * b31;
    this->m[1][1] = a21 * b12 + a22 * b22 + a23 * b32;
    this->m[1][2] = a21 * b13 + a22 * b23 + a23 * b33;
    this->m[1][3] = a21 * b14 + a22 * b24 + a23 * b34 + a24;

    this->m[2][0] = a31 * b11 + a32 * b21 + a33 * b31;
    this->m[2][1] = a31 * b12 + a32 * b22 + a33 * b32;
    this->m[2][2] = a31 * b13 + a32 * b23 + a33 * b33;
    this->m[2][3] = a31 * b14 + a32 * b24 + a33 * b34 + a34;

    this->m[3][0] = a41 * b11 + a42 * b21 + a43 * b31;
    this->m[3][1] = a41 * b12 + a42 * b22 + a43 * b32;
    this->m[3][2] = a41 * b13 + a42 * b23 + a43 * b33;
    this->m[3][3] = a41 * b14 + a42 * b24 + a43 * b34 + a44;
}

template <typename T>
inline void
Matrix44<T>::setTranspose(const Self& n)
{
    RIO_ASSERT(this != &n);

    this->m[0][0] = n.m[0][0];
    this->m[0][1] = n.m[1][0];
    this->m[0][2] = n.m[2][0];
    this->m[0][3] = n.m[3][0];

    this->m[1][0] = n.m[0][1];
    this->m[1][1] = n.m[1][1];
    this->m[1][2] = n.m[2][1];
    this->m[1][3] = n.m[3][1];

    this->m[2][0] = n.m[0][2];
    this->m[2][1] = n.m[1][2];
    this->m[2][2] = n.m[2][2];
    this->m[2][3] = n.m[3][2];

    this->m[3][0] = n.m[0][3];
    this->m[3][1] = n.m[1][3];
    this->m[3][2] = n.m[2][3];
    this->m[3][3] = n.m[3][3];
}

template <typename T>
inline void
Matrix44<T>::transpose()
{
    const T a12 = this->m[0][1];
    const T a13 = this->m[0][2];
    const T a14 = this->m[0][3];

    const T a21 = this->m[1][0];
    const T a23 = this->m[1][2];
    const T a24 = this->m[1][3];

    const T a31 = this->m[2][0];
    const T a32 = this->m[2][1];
    const T a34 = this->m[2][3];

    const T a41 = this->m[3][0];
    const T a42 = this->m[3][1];
    const T a43 = this->m[3][2];

    this->m[0][1] = a21;
    this->m[0][2] = a31;
    this->m[0][3] = a41;

    this->m[1][0] = a12;
    this->m[1][2] = a32;
    this->m[1][3] = a42;

    this->m[2][0] = a13;
    this->m[2][1] = a23;
    this->m[2][3] = a43;

    this->m[3][0] = a14;
    this->m[3][1] = a24;
    this->m[3][2] = a34;
}

template <typename T>
inline void
Matrix44<T>::makeS(const Vec3& s)
{
    this->m[0][0] = s.x;
    this->m[1][0] = 0;
    this->m[2][0] = 0;
    this->m[3][0] = 0;

    this->m[0][1] = 0;
    this->m[1][1] = s.y;
    this->m[2][1] = 0;
    this->m[3][1] = 0;

    this->m[0][2] = 0;
    this->m[1][2] = 0;
    this->m[2][2] = s.z;
    this->m[3][2] = 0;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeR(const Vec3& r)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = (cosV[1] * cosV[2]);
    this->m[1][0] = (cosV[1] * sinV[2]);
    this->m[2][0] = -sinV[1];
    this->m[3][0] = 0;

    this->m[0][1] = (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = (sinV[0] * cosV[1]);
    this->m[3][1] = 0;

    this->m[0][2] = (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = (cosV[0] * cosV[1]);
    this->m[3][2] = 0;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
    this->m[3][3] = 1;
}

template <typename T>
void Matrix44<T>::makeQ(const Quat& q)
{
    // Assuming the quaternion "q" is normalized

    const T yy = 2 * q.y * q.y;
    const T zz = 2 * q.z * q.z;
    const T xx = 2 * q.x * q.x;
    const T xy = 2 * q.x * q.y;
    const T xz = 2 * q.x * q.z;
    const T yz = 2 * q.y * q.z;
    const T wz = 2 * q.w * q.z;
    const T wx = 2 * q.w * q.x;
    const T wy = 2 * q.w * q.y;

    this->m[0][0] = 1 - yy - zz;
    this->m[0][1] =     xy - wz;
    this->m[0][2] =     xz + wy;

    this->m[1][0] =     xy + wz;
    this->m[1][1] = 1 - xx - zz;
    this->m[1][2] =     yz - wx;

    this->m[2][0] =     xz - wy;
    this->m[2][1] =     yz + wx;
    this->m[2][2] = 1 - xx - yy;

    this->m[3][0] = 0;
    this->m[3][1] = 0;
    this->m[3][2] = 0;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeT(const Vec3& t)
{
    this->m[0][0] = 1;
    this->m[1][0] = 0;
    this->m[2][0] = 0;
    this->m[3][0] = 0;

    this->m[0][1] = 0;
    this->m[1][1] = 1;
    this->m[2][1] = 0;
    this->m[3][1] = 0;

    this->m[0][2] = 0;
    this->m[1][2] = 0;
    this->m[2][2] = 1;
    this->m[3][2] = 0;

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeSR(const Vec3& s, const Vec3& r)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = s.x * (cosV[1] * cosV[2]);
    this->m[1][0] = s.x * (cosV[1] * sinV[2]);
    this->m[2][0] = s.x * -sinV[1];
    this->m[3][0] = 0;

    this->m[0][1] = s.y * (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = s.y * (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = s.y * (sinV[0] * cosV[1]);
    this->m[3][1] = 0;

    this->m[0][2] = s.z * (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = s.z * (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = s.z * (cosV[0] * cosV[1]);
    this->m[3][2] = 0;

    this->m[0][3] = 0;
    this->m[1][3] = 0;
    this->m[2][3] = 0;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeSQ(const Vec3& s, const Quat& q)
{
    makeQ(q);

    *(Vector3<T>*)(&this->m[0][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[1][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[2][0]) *= static_cast<const Vector3<T>&>(s);
}

template <typename T>
inline void
Matrix44<T>::makeST(const Vec3& s, const Vec3& t)
{
    this->m[0][0] = s.x;
    this->m[1][0] = 0;
    this->m[2][0] = 0;
    this->m[3][0] = 0;

    this->m[0][1] = 0;
    this->m[1][1] = s.y;
    this->m[2][1] = 0;
    this->m[3][1] = 0;

    this->m[0][2] = 0;
    this->m[1][2] = 0;
    this->m[2][2] = s.z;
    this->m[3][2] = 0;

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeRT(const Vec3& r, const Vec3& t)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = (cosV[1] * cosV[2]);
    this->m[1][0] = (cosV[1] * sinV[2]);
    this->m[2][0] = -sinV[1];
    this->m[3][0] = 0;

    this->m[0][1] = (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = (sinV[0] * cosV[1]);
    this->m[3][1] = 0;

    this->m[0][2] = (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = (cosV[0] * cosV[1]);
    this->m[3][2] = 0;

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeQT(const Quat& q, const Vec3& t)
{
    makeQ(q);

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix44<T>::makeSRT(const Vec3& s, const Vec3& r, const Vec3& t)
{
    const T sinV[3] = { std::sin(r.x),
                        std::sin(r.y),
                        std::sin(r.z) };

    const T cosV[3] = { std::cos(r.x),
                        std::cos(r.y),
                        std::cos(r.z) };

    this->m[0][0] = s.x * (cosV[1] * cosV[2]);
    this->m[1][0] = s.x * (cosV[1] * sinV[2]);
    this->m[2][0] = s.x * -sinV[1];
    this->m[3][0] = 0;

    this->m[0][1] = s.y * (sinV[0] * sinV[1] * cosV[2] - cosV[0] * sinV[2]);
    this->m[1][1] = s.y * (sinV[0] * sinV[1] * sinV[2] + cosV[0] * cosV[2]);
    this->m[2][1] = s.y * (sinV[0] * cosV[1]);
    this->m[3][1] = 0;

    this->m[0][2] = s.z * (cosV[0] * cosV[2] * sinV[1] + sinV[0] * sinV[2]);
    this->m[1][2] = s.z * (cosV[0] * sinV[2] * sinV[1] - sinV[0] * cosV[2]);
    this->m[2][2] = s.z * (cosV[0] * cosV[1]);
    this->m[3][2] = 0;

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
    this->m[3][3] = 1;
}

template <typename T>
inline void
Matrix44<T>::makeSQT(const Vec3& s, const Quat& q, const Vec3& t)
{
    makeQ(q);

    *(Vector3<T>*)(&this->m[0][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[1][0]) *= static_cast<const Vector3<T>&>(s);
    *(Vector3<T>*)(&this->m[2][0]) *= static_cast<const Vector3<T>&>(s);

    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix44<T>::applyScaleLocal(T s)
{
    *(Vec3*)&(this->v[0]) *= s;
    *(Vec3*)&(this->v[1]) *= s;
    *(Vec3*)&(this->v[2]) *= s;
}

template <typename T>
inline void
Matrix44<T>::applyScaleLocal(const Vec3& s)
{
    this->m[0][0] *= s.x;
    this->m[1][0] *= s.x;
    this->m[2][0] *= s.x;

    this->m[0][1] *= s.y;
    this->m[1][1] *= s.y;
    this->m[2][1] *= s.y;

    this->m[0][2] *= s.z;
    this->m[1][2] *= s.z;
    this->m[2][2] *= s.z;
}

template <typename T>
inline void
Matrix44<T>::applyScaleWorld(T s)
{
    this->v[0] *= s;
    this->v[1] *= s;
    this->v[2] *= s;
}

template <typename T>
inline void
Matrix44<T>::applyScaleWorld(const Vec3& s)
{
    this->v[0] *= s.x;
    this->v[1] *= s.y;
    this->v[2] *= s.z;
}

template <typename T>
inline void
Matrix44<T>::setTranslationWorld(const Vec3& t)
{
    this->m[0][3] = t.x;
    this->m[1][3] = t.y;
    this->m[2][3] = t.z;
}

template <typename T>
inline void
Matrix44<T>::applyTranslationWorld(const Vec3& t)
{
    this->m[0][3] += t.x;
    this->m[1][3] += t.y;
    this->m[2][3] += t.z;
}

}

#endif // RIO_MATH_MATRIX_IMPL_H
