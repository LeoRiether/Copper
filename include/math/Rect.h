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

    inline Vec2<Cart> Foot() const { return {x + w / 2, y + h}; }

    inline void SetFoot(Vec2<Cart> foot) {
        x = foot.x - w / 2;
        y = foot.y - h;
    }

    /** Assumes the width of the rect is the width of the tile, then finds the
     * center of that tile  */
    inline Vec2<Cart> IsoBaseCenter() const {
        return {x + w / 2, y + h - w / 4};
    }
};
