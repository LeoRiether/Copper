#pragma once

#include <cmath>
inline const float PI = acos(-1);

// Isometric coordinate system
struct Iso {};

// Cartesian coordinate system
struct Cart {};

template <class CoordsSystem>
struct Vec2 {
    float x, y;

    inline Vec2 GetRotated(float theta) const {
        const float cost = std::cos(theta);
        const float sint = std::sin(theta);
        return Vec2{x * cost - y * sint, x * sint + y * cost};
    }

    inline Vec2 operator+(const Vec2& rhs) const {
        return Vec2{x + rhs.x, y + rhs.y};
    }
    inline Vec2 operator-(const Vec2& rhs) const {
        return Vec2{x - rhs.x, y - rhs.y};
    }
    inline Vec2 operator*(const float s) const { return Vec2{x * s, y * s}; }
    inline Vec2 operator/(const float s) const { return Vec2{x / s, y / s}; }
    inline friend Vec2 operator*(const float s, const Vec2& v) { return v * s; }

    inline Vec2 lerp(Vec2 to, float perc) const {
        return *this + (to - *this) * perc;
    }

    inline float norm2() const { return (double)x * x + (double)y * y; }
    inline float norm() const { return sqrt((double)x * x + (double)y * y); }
    inline Vec2 normalize() const { return *this / norm(); }

    inline float angle() const { return std::atan2(y, x); }

    inline float dot(const Vec2<CoordsSystem> rhs) const {
        return x * rhs.x + y * rhs.y;
    };

    Vec2<Iso> toIso() const;
    Vec2<Cart> toCart() const;
};

template <>
Vec2<Iso> Vec2<Iso>::toIso() const;

template <>
Vec2<Iso> Vec2<Cart>::toIso() const;

template <>
Vec2<Cart> Vec2<Iso>::toCart() const;

template <>
Vec2<Cart> Vec2<Cart>::toCart() const;
