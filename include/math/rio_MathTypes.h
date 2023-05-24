#ifndef RIO_MATH_TYPES_H
#define RIO_MATH_TYPES_H

#include <misc/rio_Types.h>

namespace rio {

template <typename T>
struct BaseVec2
{
    T   x;
    T   y;
};

template <typename T>
struct BaseVec3
{
    T   x;
    T   y;
    T   z;
};

template <typename T>
struct BaseVec4
{
    T   x;
    T   y;
    T   z;
    T   w;
};

template <typename T>
struct BaseQuat
{
    T   w;
    T   x;
    T   y;
    T   z;
};

template <typename T>
struct BaseMtx22
{
    union
    {
        T           m[2][2];
        T           a[2*2];
        BaseVec2<T> v[2];
    };
};

template <typename T>
struct BaseMtx32
{
    union
    {
        T           m[3][2];
        T           a[3*2];
        BaseVec2<T> v[3];
    };
};

template <typename T>
struct BaseMtx42
{
    union
    {
        T           m[4][2];
        T           a[4*2];
        BaseVec2<T> v[4];
    };
};

template <typename T>
struct BaseMtx23
{
    union
    {
        T           m[2][3];
        T           a[2*3];
        BaseVec3<T> v[2];
    };
};

template <typename T>
struct BaseMtx33
{
    union
    {
        T           m[3][3];
        T           a[3*3];
        BaseVec3<T> v[3];
    };
};

template <typename T>
struct BaseMtx43
{
    union
    {
        T           m[4][3];
        T           a[4*3];
        BaseVec3<T> v[4];
    };
};

template <typename T>
struct BaseMtx24
{
    union
    {
        T           m[2][4];
        T           a[2*4];
        BaseVec4<T> v[2];
    };
};

template <typename T>
struct BaseMtx34
{
    union
    {
        T           m[3][4];
        T           a[3*4];
        BaseVec4<T> v[3];
    };
};

template <typename T>
struct BaseMtx44
{
    union
    {
        T           m[4][4];
        T           a[4*4];
        BaseVec4<T> v[4];
    };
};

typedef BaseVec2<s32>   BaseVec2i;
typedef BaseVec2<u32>   BaseVec2u;
typedef BaseVec2<f32>   BaseVec2f;

typedef BaseVec3<s32>   BaseVec3i;
typedef BaseVec3<u32>   BaseVec3u;
typedef BaseVec3<f32>   BaseVec3f;

typedef BaseVec4<s32>   BaseVec4i;
typedef BaseVec4<u32>   BaseVec4u;
typedef BaseVec4<f32>   BaseVec4f;

typedef BaseQuat<f32>   BaseQuatf;

typedef BaseMtx22<f32>  BaseMtx22f;
typedef BaseMtx32<f32>  BaseMtx32f;
typedef BaseMtx42<f32>  BaseMtx42f;

typedef BaseMtx23<f32>  BaseMtx23f;
typedef BaseMtx33<f32>  BaseMtx33f;
typedef BaseMtx43<f32>  BaseMtx43f;

typedef BaseMtx24<f32>  BaseMtx24f;
typedef BaseMtx34<f32>  BaseMtx34f;
typedef BaseMtx44<f32>  BaseMtx44f;

}

#endif // RIO_MATH_TYPES_H
