#pragma once

#include <array>

#include "math/Vec2.h"

using std::array;

struct Steering {
   public:
    array<float, 8> values{0};

    Steering* Add(int i, float w);
    Steering* AddTerrain(Vec2<Iso> pos);
    Vec2<Iso> Result() const;

    Vec2<Iso> direction(int i) const;
};
