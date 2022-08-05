#include <LESSR/Vector4.h>
#include <LESSR/MathUtils.h>

namespace LESSR
{

void Vector4::lerp(const Vector4& v, float a, Vector4* dst)
{
    dst->x = MATH_LERP(x, v.x, a);
    dst->y = MATH_LERP(y, v.y, a);
    dst->z = MATH_LERP(z, v.z, a);
    dst->w = MATH_LERP(w, v.w, a);
}

Vector4 Vector4::lerp(const Vector4& v, float a)
{
    return Vector4(MATH_LERP(x, v.x, a),
                   MATH_LERP(y, v.y, a),
                   MATH_LERP(z, v.z, a),
                   MATH_LERP(w, v.w, a));
}

float Vector4::dot(const Vector4& v)
{
    return (x * v.x + y * v.y + z * v.z + w * v.w);
}

float Vector4::distance(const Vector4& v)
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z; // dz nuts
    float dw = v.w - w;
    return sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

float Vector4::length()
{
    return sqrt(x * x + y * y + z * z + w * w);
}

void Vector4::normalize()
{
    float n = x * x + y * y + z * z + w * w;
    // Already normalized.
    if (n == 1.0f)
        return;

    n = sqrt(n);
    // Too close to zero.
    if (n < 2e-37f)
        return;

    n = 1.0f / n;
    x *= n;
    y *= n;
    z *= n;
    w *= n;
}

}