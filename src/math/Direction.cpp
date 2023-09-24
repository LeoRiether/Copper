#include "math/Direction.h"

#include "InputManager.h"

Direction Direction::fromInput() {
    Direction self;
    auto& input = InputManager::Instance();

    self.x = input.IsKeyDown(MOVE_LEFT_KEY)    ? Left
             : input.IsKeyDown(MOVE_RIGHT_KEY) ? Right
                                               : NoneX;

    self.y = input.IsKeyDown(MOVE_UP_KEY)     ? Up
             : input.IsKeyDown(MOVE_DOWN_KEY) ? Down
                                              : NoneY;
    return self;
}

// Compares against all 9 directions to see which has the biggest dot product
// with `v`.
// PERF: maybe we could improve the algorithm here, but probably not a
// bottleneck.
Direction Direction::approxFromVec(Vec2<Cart> v) {
    Direction self{NoneX, NoneY};
    float maxDot = 0;
    for (int8_t x = -1; x <= 1; x++) {
        for (int8_t y = -1; y <= 1; y++) {
            Direction other{(DirectionX)x, (DirectionY)y};
            float dot = v.dot(other.toVec());
            if (dot > maxDot) {
                maxDot = dot;
                self = other;
            }
        }
    }
    return self;
}

Vec2<Cart> Direction::toVec() {
    Vec2<Cart> res{(float)x, (float)y};
    if (res.x != 0 && res.y != 0) {
        res.x *= 2;          // walk on the same angle as the tiles
        res = res / SQRT_5;  // normalize
    }
    return res;
}

string Direction::toString() {
    string xstr = x == Left ? "W" : x == Right ? "E" : "";
    string ystr = y == Up ? "N" : y == Down ? "S" : "";
    return ystr + xstr;
}