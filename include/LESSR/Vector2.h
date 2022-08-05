#pragma once

namespace LESSR
{
struct Vector3;
struct Vector4;
struct Vector2
{
    float x;
    float y;

    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector3& v);
    Vector2(const Vector4& v);

    void set(float x, float y);
    void zero();
    void lerp(const Vector2& v, float a, Vector2* dst);
    Vector2 lerp(const Vector2& v, float a);
    float dot(const Vector2& v);
    float distance(const Vector2& v);
    float length();

    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(const Vector2& v) const;
    Vector2 operator/(const Vector2& v) const;
    Vector2& operator+=(const Vector2& v);
    Vector2& operator-=(const Vector2& v);
    Vector2& operator*=(const Vector2& v);
    Vector2& operator/=(const Vector2& v);

    Vector2 operator*(float f) const;
    Vector2 operator/(float f) const;
    Vector2& operator*=(float f);
    Vector2& operator/=(float f);

    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;

    Vector2 operator-();
};

inline Vector2::Vector2() : x(0.f), y(0.f) {};
inline Vector2::Vector2(float x, float y) : x(x), y(y) {};

inline void Vector2::set(float _x, float _y)
{
    x = _x;
    y = _y;
}

inline void Vector2::zero()
{
    x = 0;
    y = 0;
}

inline Vector2 Vector2::operator+(const Vector2& v) const
{
    return Vector2(x + v.x, y + v.y);
}

inline Vector2 Vector2::operator-(const Vector2& v) const
{
    return Vector2(x - v.x, y - v.y);
}

inline Vector2 Vector2::operator*(const Vector2& v) const
{
    return Vector2(x * v.x, y * v.y);
}

inline Vector2 Vector2::operator/(const Vector2& v) const
{
    return Vector2(x / v.x, y / v.y);
}

inline Vector2& Vector2::operator+=(const Vector2& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

inline Vector2& Vector2::operator/=(const Vector2& v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

inline Vector2 Vector2::operator*(float f) const
{
    return Vector2(x * f, y * f);
}

inline Vector2 Vector2::operator/(float f) const
{
    return Vector2(x / f, y / f);
}

inline Vector2& Vector2::operator*=(float f)
{
    x *= f;
    y *= f;
    return *this;
}

inline Vector2& Vector2::operator/=(float f)
{
    x /= f;
    y /= f;
    return *this;
}

inline bool Vector2::operator==(const Vector2& v) const
{
    return v.x == x && v.y == y;
}

inline bool Vector2::operator!=(const Vector2& v) const
{
    return v.x != x || v.y != y;
}

inline Vector2 Vector2::operator-()
{
    return Vector2(-x, -y);
}

}