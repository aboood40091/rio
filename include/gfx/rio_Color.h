#ifndef RIO_GFX_COLOR_H
#define RIO_GFX_COLOR_H

#include <math/rio_MathTypes.h>

namespace rio {

struct Color4f
{
    void setf(f32 _r, f32 _g, f32 _b, f32 _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    union
    {
        struct
        {
            f32         r;
            f32         g;
            f32         b;
            f32         a;
        };
        f32         c[4];
        BaseVec4f   v;
    };

    static Color4f lerp(const Color4f& from, const Color4f& to, f32 ratio);
    void setLerp(const Color4f& from, const Color4f& to, f32 ratio);

    Color4f& operator+=(const Color4f& rhs);
    Color4f& operator*=(const Color4f& rhs);

    friend Color4f operator+(const Color4f& lhs, const Color4f& rhs);
    friend Color4f operator*(const Color4f& lhs, const Color4f& rhs);

    friend bool operator==(const Color4f& lhs, const Color4f& rhs);

    static constexpr f32    cElementMin = 0.0f;
    static constexpr f32    cElementMax = 1.0f;

    static const Color4f    cBlack;
    static const Color4f    cGray;
    static const Color4f    cWhite;
    static const Color4f    cRed;
    static const Color4f    cGreen;
    static const Color4f    cBlue;
    static const Color4f    cYellow;
    static const Color4f    cMagenta;
    static const Color4f    cCyan;
};

inline Color4f Color4f::lerp(const Color4f& from, const Color4f& to, f32 ratio)
{
    Color4f color;
    color.setLerp(from, to, ratio);
    return color;
}

}

#endif // RIO_GFX_COLOR_H
