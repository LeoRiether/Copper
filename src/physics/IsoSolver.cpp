#include "physics/IsoSolver.h"

Vec2<Cart> IsoSolver::Solve(const IsoCollider& collider, Vec2<Cart> point) {
    return Solve(collider, point.toIso()).toCart();
}

Vec2<Iso> IsoSolver::Solve(const IsoCollider& collider, Vec2<Iso> point) {
    const auto& box = collider.box;

    if (point.x < box.x || point.x > box.x + box.w) return point;
    if (point.y < box.y || point.y > box.y + box.h) return point;

    float left = abs(point.x - box.x);
    float right = abs(point.x - (box.x + box.w));
    float up = abs(point.y - box.y);
    float down = abs(point.y - (box.y + box.h));

    // I'm so sorry
    if (left <= right && left <= up && left <= down)
        point.x = box.x;
    else if (right <= up && right <= down)
        point.x = box.x + box.w;
    else if (up <= down)
        point.y = box.y;
    else
        point.y = box.y + box.h;

    return point;
}
