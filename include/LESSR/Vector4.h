#pragma once
#include <LESSR/Vector3.h>
#include <LESSR/Vector2.h>

namespace LESSR
{
struct Vector4
{
    float x;
    float y;
    float z;
    float w;

    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector3& v);
    Vector4(const Vector2& v);

    void set(float _x, float _y, float _z, float _w);
    void set(const Vector3& v);
    void set(const Vector2& v);
    void zero();
    void lerp(const Vector4& v, float a, Vector4* dst);
    Vector4 lerp(const Vector4& v, float a);
    float dot(const Vector4& v);
    float distance(const Vector4& v);
    float length();
    void normalize();

    Vector4 operator+(const Vector4& v) const;
    Vector4 operator-(const Vector4& v) const;
    Vector4 operator*(const Vector4& v) const;
    Vector4 operator/(const Vector4& v) const;
    Vector4& operator+=(const Vector4& v);
    Vector4& operator-=(const Vector4& v);
    Vector4& operator*=(const Vector4& v);
    Vector4& operator/=(const Vector4& v);

    Vector4 operator*(float f) const;
    Vector4 operator/(float f) const;
    Vector4& operator*=(float f);
    Vector4& operator/=(float f);

    bool operator==(const Vector4& v) const;
    bool operator!=(const Vector4& v) const;

    Vector4 operator-();
};

inline Vector4::Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {};
inline Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
inline Vector4::Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.f) {};
inline Vector4::Vector4(const Vector2& v) : x(v.x), y(v.y), z(0.f), w(1.f) {};

inline void Vector4::set(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

inline void Vector4::set(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1.f;
}

inline void Vector4::set(const Vector2& v)
{
    x = v.x;
    y = v.y;
    z = 0.f;
    w = 1.f;
}

inline void Vector4::zero()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
};

inline Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

inline Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline Vector4 Vector4::operator*(const Vector4& v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

inline Vector4 Vector4::operator/(const Vector4& v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

inline Vector4& Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

inline Vector4& Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

inline Vector4& Vector4::operator*=(const Vector4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

inline Vector4& Vector4::operator/=(const Vector4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

inline Vector4 Vector4::operator*(float f) const
{
    return Vector4(x * f, y * f, z * f, w * f);
}

inline Vector4 Vector4::operator/(float f) const
{
    return Vector4(x / f, y / f, z / f, w / f);
}

inline Vector4& Vector4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

inline Vector4& Vector4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;
    return *this;
}

inline bool Vector4::operator==(const Vector4& v) const
{
    return v.x == x && v.y == y && v.z == z && v.w == w;
};

inline bool Vector4::operator!=(const Vector4& v) const
{
    return v.x != x || v.y != y || v.z != z || v.w != w;
}

inline Vector4 Vector4::operator-()
{
    return Vector4(-x, -y, -z, -w);
}

}