#include <math/rio_Quat.h>

namespace rio {

#if RIO_IS_CAFE

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
void
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

template <>
f32
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

template <>
f32
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

template <>
void
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

}
