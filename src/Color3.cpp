#include <LESSR/Color3.h>
#include <LESSR/Color4.h>
#include <LESSR/MathUtils.h>

namespace LESSR
{

Color3::Color3(const Color4& c)
: r(c.r),
  g(c.g),
  b(c.b)
{}

void Color3::lerp(const Color3& v, float a, Color3* dst)
{
    dst->r = MATH_LERP(r, v.r, a);
    dst->g = MATH_LERP(g, v.g, a);
    dst->b = MATH_LERP(b, v.b, a);
}

inline Color3 Color3::lerp(const Color3& v, float a)
{
    return Color3(MATH_LERP(r, v.r, a),
                  MATH_LERP(g, v.g, a),
                  MATH_LERP(b, v.b, a));
}

}