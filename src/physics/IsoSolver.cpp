#include "physics/IsoSolver.h"

#include <cmath>

#define MODULE "IsoSolver"

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

Rect IsoSolver::Solve(Rect A, Rect prevA, const Rect& B) {
    auto inside = [&](Rect a, Rect b) {
        constexpr float eps = 0.01;
        return !(a.x + eps > b.x + b.w || a.x + a.w < b.x + eps ||
                 a.y + eps > b.y + b.h || a.y + a.h < b.y + eps);
    };

    if (!inside(A, B)) return A;

    auto xmove = prevA;
    xmove.OffsetBy({A.x - prevA.x, 0});
    auto ymove = prevA;
    ymove.OffsetBy({0, A.y - prevA.y});
    if (inside(xmove, B)) {
        float left = abs(A.x + A.w - B.x);
        float right = abs(B.x + B.w - A.x);
        A.x = left <= right ? (B.x - A.w) : (B.x + B.w);
    }
    if (inside(ymove, B)) {
        float up = abs(A.y + A.h - B.y);
        float down = abs(B.y + B.h - A.y);
        A.y = up <= down ? (B.y - A.h) : (B.y + B.h);
    }

    return A;
}
