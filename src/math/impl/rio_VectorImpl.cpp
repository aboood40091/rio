#include <math/rio_Vector.h>

namespace rio {

#if RIO_IS_CAFE

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
f32
Vector2<f32>::squaredLength() const
{
    const Vector2f* const pSelf = this;

    // Temporary variable
    f32 v0;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Multiply ps(v0) by itself
    asm volatile ("ps_mul %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    // Add ps1(v0) to ps0(v0)
    asm volatile ("ps_sum0 %[v0], %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    return v0;
}

template <>
f32
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
f32
Vector3<f32>::squaredLength() const
{
    const Vector3f* const pSelf = this;

    // Temporary variables
    f32 v0;

    // Load 2 floats from pSelf into ps(v0)
    asm volatile ("psq_l %[v0], 0(%[pSelf]), 0, 0" : [v0] "=f"(v0) : [pSelf] "r"(pSelf));

    // Multiply ps(v0) by itself
    asm volatile ("ps_mul %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    // Add ps1(v0) to ps0(v0)
    asm volatile ("ps_sum0 %[v0], %[v0], %[v0], %[v0]" : [v0] "+f"(v0));

    // Handle z component
    v0 += Mathf::square(pSelf->z);

    return v0;
}

template <>
f32
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
f32
Vector4<f32>::squaredLength() const
{
    const Vector4f* const pSelf = this;

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

template <>
f32
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

template <>
void
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

template <>
void
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
