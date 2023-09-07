#pragma once

#include "Vec2.h"

struct Rect {
    float x, y, w, h;

    bool Contains(const Vec2 p);
    inline Vec2 Center() const { return Vec2{x + w / 2, y + h / 2}; }
    inline void SetCenter(Vec2 center) {
        x = center.x - w / 2;
        y = center.y - h / 2;
    }
};
