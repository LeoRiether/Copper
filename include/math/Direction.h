#pragma once

#include <string>
#include <vector>

#include "math/Vec2.h"
using std::string;
using std::vector;

// it's a normalizing factor in toVec() :)
constexpr float SQRT_5 = 2.23606797749979f;

enum DirectionX { NoneX = 0, Left = -1, Right = 1 };
enum DirectionY { NoneY = 0, Up = -1, Down = 1 };

struct Direction {
    DirectionX x;
    DirectionY y;

    /** Constructs a Direction from the keys the user is currently pressing */
    static Direction fromInput();

    /** Constructs the Direction that is closest in angle to the given Vec2 */
    static Direction approxFromVec(Vec2<Cart> v);

    /** Returns a normalized vector with the appropriate direction */
    Vec2<Cart> toVec() const;

    string toString() const;

    bool isNone() const;

    bool operator==(Direction& rhs) const;
    bool operator!=(Direction& rhs) const;
    Direction operator-() const;
};
