#include <gfx/rio_Color.h>

#include <algorithm>

namespace rio {

const Color4f Color4f::cBlack  { 0.0f, 0.0f, 0.0f, 1.0f };
const Color4f Color4f::cGray   { 0.5f, 0.5f, 0.5f, 1.0f };
const Color4f Color4f::cWhite  { 1.0f, 1.0f, 1.0f, 1.0f };
const Color4f Color4f::cRed    { 1.0f, 0.0f, 0.0f, 1.0f };
const Color4f Color4f::cGreen  { 0.0f, 1.0f, 0.0f, 1.0f };
const Color4f Color4f::cBlue   { 0.0f, 0.0f, 1.0f, 1.0f };
const Color4f Color4f::cYellow { 1.0f, 1.0f, 0.0f, 1.0f };
const Color4f Color4f::cMagenta{ 1.0f, 0.0f, 1.0f, 1.0f };
const Color4f Color4f::cCyan   { 0.0f, 1.0f, 1.0f, 1.0f };

void Color4f::setLerp(const Color4f& from, const Color4f& to, f32 ratio)
{
    ratio = std::clamp(ratio, cElementMin, cElementMax);
    a = (1 - ratio) * from.a + ratio * to.a;
    r = (1 - ratio) * from.r + ratio * to.r;
    g = (1 - ratio) * from.g + ratio * to.g;
    b = (1 - ratio) * from.b + ratio * to.b;
}

}
