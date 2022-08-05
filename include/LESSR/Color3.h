#pragma once
#include <cstdint>

namespace LESSR
{
struct Color4;
struct Color3
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color3();
    Color3(uint8_t r, uint8_t g, uint8_t b);
    Color3(uint8_t c[3]);
    Color3(const Color4& c);

    void set(uint8_t r, uint8_t g, uint8_t b);
    void lerp(const Color3& v, float a, Color3* dst);
    Color3 lerp(const Color3& v, float a);

    bool operator==(const Color3& v) const;

    Color3 operator*(const Color3& v) const;
    Color3& operator*=(const Color3& v);

    Color3 operator*(float f) const;
    Color3& operator*=(float f);

    Color3 operator+(const Color3& v) const;
    Color3& operator+=(const Color3& v);

    Color3 operator-(const Color3& v) const;
    Color3& operator-=(const Color3& v);
};

inline Color3::Color3() : r(0), g(0), b(0) {};
inline Color3::Color3(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};
inline Color3::Color3(uint8_t c[3]) : r(c[0]), g(c[1]), b(c[2]) {};

inline void Color3::set(uint8_t _r, uint8_t _g, uint8_t _b)
{
    r = _r;
    g = _g;
    b = _b;
}

inline bool Color3::operator==(const Color3& v) const
{
    return v.r == r && v.g == g && v.b == b;
}

inline Color3 Color3::operator*(const Color3& v) const
{
    return Color3(*this) *= v;
}

inline Color3& Color3::operator*=(const Color3& v)
{
    r *= v.r/255.f;
    g *= v.g/255.f;
    b *= v.b/255.f;
    return *this;
}

inline Color3 Color3::operator*(float f) const
{
    return Color3(*this) *= f;
}

inline Color3& Color3::operator*=(float f)
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

inline Color3 Color3::operator+(const Color3& v) const
{
    return Color3(*this) += v;
}

inline Color3& Color3::operator+=(const Color3& v)
{
    r += v.r;
    g += v.g;
    b += v.b;
    return *this;
}

inline Color3 Color3::operator-(const Color3& v) const
{
    return Color3(*this) -= v;
}

inline Color3& Color3::operator-=(const Color3& v)
{
    r -= v.r;
    g -= v.g;
    b -= v.b;
    return *this;
}

}