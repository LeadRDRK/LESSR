#include <LESSR/Color4.h>
#include <LESSR/MathUtils.h>

namespace LESSR
{

void Color4::lerp(const Color4& v, float _a, Color4* dst)
{
    dst->r = MATH_LERP(r, v.r, _a);
    dst->g = MATH_LERP(g, v.g, _a);
    dst->b = MATH_LERP(b, v.b, _a);
    dst->a = MATH_LERP(a, v.a, _a);
}

inline Color4 Color4::lerp(const Color4& v, float _a)
{
    return Color4(MATH_LERP(r, v.r, _a),
                  MATH_LERP(g, v.g, _a),
                  MATH_LERP(b, v.b, _a),
                  MATH_LERP(a, v.a, _a));
}

}