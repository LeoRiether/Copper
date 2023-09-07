#pragma once

#include <cmath>
inline const float PI = acos(-1);

struct Vec2 {
    float x, y;

    Vec2 GetRotated(float theta) const;
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
};
