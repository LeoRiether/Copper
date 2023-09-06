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
};
