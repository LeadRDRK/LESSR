#include <LESSR/Vector3.h>
#include <LESSR/MathUtils.h>

namespace LESSR
{

void Vector3::lerp(const Vector3& v, float a, Vector3* dst)
{
    dst->x = MATH_LERP(x, v.x, a);
    dst->y = MATH_LERP(y, v.y, a);
    dst->z = MATH_LERP(z, v.z, a);
}

Vector3 Vector3::lerp(const Vector3& v, float a)
{
    return Vector3(MATH_LERP(x, v.x, a),
                   MATH_LERP(y, v.y, a),
                   MATH_LERP(z, v.z, a));
}

float Vector3::dot(const Vector3& v)
{
    return (x * v.x + y * v.y + z * v.z);
}

void Vector3::cross(const Vector3& v, Vector3* dst)
{
    dst->x = (y * v.z) - (z * v.y);
    dst->y = (z * v.x) - (x * v.z);
    dst->z = (x * v.y) - (y * v.x);
}

Vector3 Vector3::cross(const Vector3& v)
{
    return Vector3((y * v.z) - (z * v.y),
                   (z * v.x) - (x * v.z),
                   (x * v.y) - (y * v.x));
}

float Vector3::distance(const Vector3& v)
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z; // dz nuts
    return sqrt(dx * dx + dy * dy + dz * dz);
}

float Vector3::length()
{
    return sqrt(x * x + y * y + z * z);
}

void Vector3::normalize()
{
    float n = x * x + y * y + z * z;
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
}

}