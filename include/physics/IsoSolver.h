#pragma once

#include "component/IsoCollider.h"
#include "math/Vec2.h"

class IsoSolver {
public:
  static Vec2<Cart> Solve(const IsoCollider &collider, Vec2<Cart> point);
  static Vec2<Iso> Solve(const IsoCollider &collider, Vec2<Iso> point);
};
