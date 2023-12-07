#pragma once

#include "component/IsoCollider.h"
#include "math/Vec2.h"

class IsoSolver {
   public:
    static Vec2<Cart> Solve(const IsoCollider& collider, Vec2<Cart> point,
                            Vec2<Cart> prevFrame);
    static Vec2<Iso> Solve(const IsoCollider& collider, Vec2<Iso> point,
                           Vec2<Iso> prevFrame);

    /** Collide A against B. A moves, B does not. */
    static Rect Solve(Rect A, Rect prevA, const Rect& B);
};
