#pragma once

#include "Vec2.h"

struct Rect {
    float x, y, w, h;

    bool Contains(const Vec2 p);
};
