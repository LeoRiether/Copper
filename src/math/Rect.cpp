#include "math/Rect.h"

bool Rect::Contains(const Vec2<Cart> p) {
    return p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h;
}
