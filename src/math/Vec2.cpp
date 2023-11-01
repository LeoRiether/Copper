#include "math/Vec2.h"

template <>
Vec2<Iso> Vec2<Iso>::toIso() const {
    return {x, y};
}

template <>
Vec2<Iso> Vec2<Cart>::toIso() const {
    return {x + 2.0f * y, -x + 2.0f * y};
}

template <>
Vec2<Cart> Vec2<Iso>::toCart() const {
    return {(x - y) / 2.0f, (x + y) / 4.0f};
}

template <>
Vec2<Cart> Vec2<Cart>::toCart() const {
    return {x, y};
}
