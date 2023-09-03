#include "Vec2.h"

#include <cmath>

Vec2 Vec2::GetRotated(float theta) const {
    const float cost = std::cos(theta);
    const float sint = std::sin(theta);
    return Vec2{x * cost - y * sint, x * sint + y * cost};
}

Vec2 Vec2::operator+(const Vec2& rhs) const {
    return Vec2{x + rhs.x, y + rhs.y};
}
