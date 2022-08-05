#include <LESSR/Vector2.h>
#include <LESSR/Vector3.h>
#include <LESSR/Vector4.h>
#include <LESSR/MathUtils.h>

namespace LESSR
{

Vector2::Vector2(const Vector3& v)
: x(v.x),
  y(v.y)
{}

Vector2::Vector2(const Vector4& v)
: x(v.x),
  y(v.y)
{}

void Vector2::lerp(const Vector2& v, float a, Vector2* dst)
{
    dst->x = MATH_LERP(x, v.x, a);
    dst->y = MATH_LERP(y, v.y, a);
}

Vector2 Vector2::lerp(const Vector2& v, float a)
{
    return Vector2(MATH_LERP(x, v.x, a),
                   MATH_LERP(y, v.y, a));
}

float Vector2::dot(const Vector2& v)
{
    return (x * v.x + y * v.y);
}

float Vector2::distance(const Vector2& v)
{
    float dx = v.x - x;
    float dy = v.y - y;
    return sqrt(dx * dx + dy * dy);
}

float Vector2::length()
{
    return sqrt(x * x + y * y);
}

}