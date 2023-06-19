#ifndef RIO_MATH_MATRIX_H
#define RIO_MATH_MATRIX_H

#include <math/rio_MathTypes.h>

namespace rio {

template <typename T>
struct Matrix34 : BaseMtx34<T>
{
    typedef Matrix34<T> Self;
    typedef BaseQuat<T> Quat;
    typedef BaseVec3<T> Vec3;

    bool operator==(const Self& n) const;
    bool operator!=(const Self& n) const { return !operator==(n); }

    Self operator+(const Self& n) const;
    Self operator-(const Self& n) const;

    Self operator*(T s) const;
    Self operator/(T s) const;

    Self& operator+=(const Self& n);
    Self& operator-=(const Self& n);

    Self& operator*=(T s);
    Self& operator/=(T s);

    bool setInverse(const Self& n);
    bool setInverseTranspose(const Self& n);
    void setMul(const Self& a, const Self& b);
    void setTranspose(const Self& n);
    void transpose();

    void makeS(const Vec3& s);
    void makeR(const Vec3& r);
    void makeQ(const Quat& q);
    void makeT(const Vec3& t);
    void makeSR(const Vec3& s, const Vec3& r);
    void makeSQ(const Vec3& s, const Quat& q);
    void makeST(const Vec3& s, const Vec3& t);
    void makeRT(const Vec3& r, const Vec3& t);
    void makeQT(const Quat& q, const Vec3& t);
    void makeSRT(const Vec3& s, const Vec3& r, const Vec3& t);
    void makeSQT(const Vec3& s, const Quat& q, const Vec3& t);

    bool makeVectorRotation(const Vec3& from, const Vec3& to);

    // TODO
    //void setScaleLocal(T s);
    //void setScaleLocal(const Vec3& s);
    void applyScaleLocal(T s);
    void applyScaleLocal(const Vec3& s);

    // TODO
    //void setScaleWorld(T s);
    //void setScaleWorld(const Vec3& s);
    void applyScaleWorld(T s);
    void applyScaleWorld(const Vec3& s);

    // TODO
    //void setRotationLocal(const Vec3& r);
    //void applyRotationLocal(const Vec3& r);

    // TODO
    //void setRotationWorld(const Vec3& r);
    //void applyRotationWorld(const Vec3& r);

    void applyTranslationLocal(const Vec3& t);

    void setTranslationWorld(const Vec3& t);
    void applyTranslationWorld(const Vec3& t);

    static const Self zero;
    static const Self ident;
};

typedef Matrix34<f32> Matrix34f;
typedef Matrix34<f32> Mtx34f;

template <>
const Matrix34f Matrix34f::zero;

template <>
const Matrix34f Matrix34f::ident;

template <typename T>
struct Matrix44 : BaseMtx44<T>
{
    typedef Matrix44<T> Self;
    typedef BaseMtx34<T> Mtx34;
    typedef BaseQuat<T> Quat;
    typedef BaseVec3<T> Vec3;
    typedef BaseVec4<T> Vec4;

    void fromMatrix34(const Mtx34& mtx34, T a30 = 0, T a31 = 0, T a32 = 0, T a33 = 1)
    {
        *(Mtx34*)this = mtx34;
        this->m[3][0] = a30;
        this->m[3][1] = a31;
        this->m[3][2] = a32;
        this->m[3][3] = a33;
    }

    void fromMatrix34(const Mtx34& mtx34, const Vec4& vw)
    {
        *(Mtx34*)this = mtx34;
        this->v[3] = vw;
    }

    bool operator==(const Self& n) const;
    bool operator!=(const Self& n) const { return !operator==(n); }

    Self operator+(const Self& n) const;
    Self operator-(const Self& n) const;

    Self operator*(T s) const;
    Self operator/(T s) const;

    Self& operator+=(const Self& n);
    Self& operator-=(const Self& n);

    Self& operator*=(T s);
    Self& operator/=(T s);

    bool setInverse(const Self& n);
    void setMul(const Self& a, const Self& b);
    void setMul(const Mtx34& a, const Self& b);
    void setMul(const Self& a, const Mtx34& b);
    void setTranspose(const Self& n);
    void transpose();

    void makeS(const Vec3& s);
    void makeR(const Vec3& r);
    void makeQ(const Quat& q);
    void makeT(const Vec3& t);
    void makeSR(const Vec3& s, const Vec3& r);
    void makeSQ(const Vec3& s, const Quat& q);
    void makeST(const Vec3& s, const Vec3& t);
    void makeRT(const Vec3& r, const Vec3& t);
    void makeQT(const Quat& q, const Vec3& t);
    void makeSRT(const Vec3& s, const Vec3& r, const Vec3& t);
    void makeSQT(const Vec3& s, const Quat& q, const Vec3& t);

    // TODO
    //void setScaleLocal(T s);
    //void setScaleLocal(const Vec3& s);
    void applyScaleLocal(T s);
    void applyScaleLocal(const Vec3& s);

    // TODO
    //void setScaleWorld(T s);
    //void setScaleWorld(const Vec3& s);
    void applyScaleWorld(T s);
    void applyScaleWorld(const Vec3& s);

    // TODO
    //void setRotationLocal(const Vec3& r);
    //void applyRotationLocal(const Vec3& r);

    // TODO
    //void setRotationWorld(const Vec3& r);
    //void applyRotationWorld(const Vec3& r);

    void setTranslationWorld(const Vec3& t);
    void applyTranslationWorld(const Vec3& t);

    static const Self zero;
    static const Self ident;
};

typedef Matrix44<f32> Matrix44f;
typedef Matrix44<f32> Mtx44f;

template <>
const Matrix44f Matrix44f::zero;

template <>
const Matrix44f Matrix44f::ident;

}

#include <math/impl/rio_MatrixImpl.h>

#endif // RIO_MATH_MATRIX_H
