#ifndef RIO_MATH_H
#define RIO_MATH_H

#include <misc/rio_Types.h>

#include <cmath>
#include <limits>

namespace rio {

template <typename T>
class Math
{
public:
    // Good for temporary values
    static T square(T x) { return x * x; }
    static T cube(T x) { return x * x * x; }

    static T min();
    static T max();

    static T abs(T x)
    {
        if (x > 0) return  x;
        else       return -x;
    }
};

typedef Math<s32> Mathi;
typedef Math<u32> Mathu;

template <>
class Math<f32>
{
public:
    static f32 piHalf() { return 1.570796326794896619231; }
    static f32 pi()     { return 3.141592653589793238462; }
    static f32 pi2()    { return 6.283185307179586476924; }
    static f32 ln2()    { return 0.69314718055994530942 ; }
    static f32 ln2Inv() { return 1.442695040888963407   ; }

    static f32 sqrt(f32 x);
    static f32 rsqrt(f32 x);

    // Good for temporary values
    static f32 square(f32 x) { return x * x; }
    static f32 cube(f32 x) { return x * x * x; }

    static f32 min();
    static f32 max();

    static f32 abs(f32 x);

    static f32 deg2rad(f32 a) { return a * pi() / 180 ; }
    static f32 rad2deg(f32 a) { return a * 180  / pi(); }
};

typedef Math<f32> Mathf;

template <typename T>
inline T
Math<T>::min()
{
    return std::numeric_limits<T>::min();
}

template <typename T>
inline T
Math<T>::max()
{
    return std::numeric_limits<T>::max();
}

inline f32
Mathf::min()
{
    return -std::numeric_limits<f32>::max();
}

inline f32
Mathf::max()
{
    return std::numeric_limits<f32>::max();
}

template <>
inline double
Math<double>::min()
{
    return -std::numeric_limits<double>::max();
}

template <>
inline double
Math<double>::max()
{
    return std::numeric_limits<double>::max();
}

template <>
inline long double
Math<long double>::min()
{
    return -std::numeric_limits<long double>::max();
}

template <>
inline long double
Math<long double>::max()
{
    return std::numeric_limits<long double>::max();
}

template <>
inline s32
Math<s32>::abs(s32 x)
{
    return (x ^ x >> 31) - (x >> 31);
}

template <>
inline u32
Math<u32>::abs(u32 x)
{
    return x;
}

inline f32
Mathf::abs(f32 x)
{
    return std::fabs(x);
}

template <>
inline double
Math<double>::abs(double x)
{
    return std::fabs(x);
}

template <>
inline long double
Math<long double>::abs(long double x)
{
    return std::fabs(x);
}

#if RIO_IS_CAFE

inline f32
Math<f32>::sqrt(f32 x)
{
    return rsqrt(x) * x;
}

inline f32
Math<f32>::rsqrt(f32 x)
{
    // Optimized, inline assembly version

    RIO_ASSERT(x >= 0.0f);

    if (x <= 0.0f)
        return 0.0f;

    // Temporary registers
    f32 v0;
    f32 v1;
    f32 v2;

    // Constants
    const f32 HALF  = 0.5f;
    const f32 THREE = 3.0f;

    asm ("frsqrte %[v0], %[x]                 " : [v0] "=f"(v0) : [ x] "f"( x));
    asm ("fmuls   %[v1], %[v0], %[v0]         " : [v1] "=f"(v1) : [v0] "f"(v0));
    asm ("fmuls   %[v2], %[v0], %[HALF]       " : [v2] "=f"(v2) : [v0] "f"(v0), [ HALF] "f"( HALF));
    asm ("fnmsubs %[v1], %[v1], %[x], %[THREE]" : [v1] "+f"(v1) : [ x] "f"( x), [THREE] "f"(THREE));
    asm ("fmuls   %[v0], %[v1], %[v2]         " : [v0] "=f"(v0) : [v1] "f"(v1), [   v2] "f"(   v2));

    return v0;
}

#else

inline f32
Math<f32>::sqrt(f32 x)
{
    return std::sqrt(x);
}

inline f32
Math<f32>::rsqrt(f32 x)
{
    return 1 / std::sqrt(x);
}

#endif

}

#endif // RIO_MATH_H
