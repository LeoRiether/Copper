#include "math/Vec2.h"

constexpr double SIN_PI_3 = 0.8660254037844386;

template <>
Vec2<Iso> Vec2<Iso>::toIso() const {
    return {x, y};
}

template <>
Vec2<Iso> Vec2<Cart>::toIso() const {
    return {float(0.5 / SIN_PI_3 * x + y), float(-0.5 / SIN_PI_3 * x + y)};
}

template <>
Vec2<Cart> Vec2<Iso>::toCart() const {
    return {float(SIN_PI_3 * (x - y)), 0.5f * (x + y)};
}

template <>
Vec2<Cart> Vec2<Cart>::toCart() const {
    return {x, y};
}
