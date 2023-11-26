#include "physics/IsoSolver.h"

#include <cmath>
#include <iterator>

#include "util.h"

#define MODULE "IsoSolver"

bool AAA = true;

Vec2<Cart> IsoSolver::Solve(const IsoCollider& collider, Vec2<Cart> point,
                            Vec2<Cart> prevFrame) {
    return Solve(collider, point.toIso(), prevFrame.toIso()).toCart();
}

Vec2<Iso> IsoSolver::Solve(const IsoCollider& collider, Vec2<Iso> point,
                           Vec2<Iso> prevFrame) {
    const auto& box = collider.box;

    auto inside = [&](Vec2<Iso> p, Rect r) {
        constexpr float eps = 0.005;
        return p.x >= r.x + eps && p.x + eps <= r.x + r.w && p.y >= r.y + eps &&
               p.y + eps <= r.y + r.h;
    };

    if (!inside(point, box)) return point;

    // auto pos = [&](float x) {
    //     if (x < -0.001) return INFINITY;
    //     return x;
    // };
    float left = abs(box.x - point.x);
    float right = abs(point.x - (box.x + box.w));
    float up = abs(box.y - point.y);
    float down = abs(point.y - (box.y + box.h));

    auto delta = point - prevFrame;
    auto xmove = prevFrame + Vec2<Iso>{delta.x, 0};
    auto ymove = prevFrame + Vec2<Iso>{0, delta.y};
    if (inside(xmove, box)) {
        point.x = left <= right ? box.x : (box.x + box.w);
    }
    if (inside(ymove, box)) {
        point.y = up <= down ? box.y : (box.y + box.h);
    }

    return point;
}
