#pragma once

#include "math/Vec2.h"

struct Rect {
    float x, y, w, h;

    bool Contains(const Vec2<Cart> p);
    inline Vec2<Cart> Center() const {
        return Vec2<Cart>{x + w / 2, y + h / 2};
    }
    inline void SetCenter(Vec2<Cart> center) {
        x = center.x - w / 2;
        y = center.y - h / 2;
    }
};
