#pragma once
#include <LESSR/Vector2.h>
#include <LESSR/Matrix4.h>

namespace LESSR
{
struct Vector3
{
    float x;
    float y;
    float z;

    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector2& v);

    void set(float _x, float _y, float _z);
    void set(const Vector2& v);
    void zero();
    void lerp(const Vector3& v, float a, Vector3* dst);
    Vector3 lerp(const Vector3& v, float a);
    float dot(const Vector3& v);
    void cross(const Vector3& v, Vector3* dst);
    Vector3 cross(const Vector3& v);
    float distance(const Vector3& v);
    float length();
    void normalize();

    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(const Vector3& v) const;
    Vector3 operator/(const Vector3& v) const;
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(const Vector3& v);
    Vector3& operator/=(const Vector3& v);

    Vector3 operator*(float f) const;
    Vector3 operator/(float f) const;
    Vector3& operator*=(float f);
    Vector3& operator/=(float f);

    bool operator==(const Vector3& v) const;
    bool operator!=(const Vector3& v) const;

    Vector3 operator-();
};

inline Vector3::Vector3() : x(0.f), y(0.f), z(0.f) {};
inline Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
inline Vector3::Vector3(const Vector2& v) : x(v.x), y(v.y), z(0.f) {};

inline void Vector3::set(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

inline void Vector3::set(const Vector2& v)
{
    x = v.x;
    y = v.y;
    z = 0.f;
}

inline void Vector3::zero()
{
    x = 0;
    y = 0;
    z = 0;
};

inline Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3 Vector3::operator*(const Vector3& v) const
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

inline Vector3 Vector3::operator/(const Vector3& v) const
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

inline Vector3& Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

inline Vector3& Vector3::operator/=(const Vector3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

inline Vector3 Vector3::operator*(float f) const
{
    return Vector3(x * f, y * f, z * f);
}

inline Vector3 Vector3::operator/(float f) const
{
    return Vector3(x / f, y / f, z / f);
}

inline Vector3& Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

inline Vector3& Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

inline bool Vector3::operator==(const Vector3& v) const
{
    return v.x == x && v.y == y && v.z == z;
};

inline bool Vector3::operator!=(const Vector3& v) const
{
    return v.x != x || v.y != y || v.z != z;
}

inline Vector3 Vector3::operator-()
{
    return Vector3(-x, -y, -z);
}

}