#pragma once

#include <cmath>
inline const float PI = acos(-1);

struct Vec2 {
    float x, y;

    Vec2 GetRotated(float theta) const;
    Vec2 operator+(const Vec2& rhs) const;
};
