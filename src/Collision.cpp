#include "Collision.h"

bool Collision::IsColliding(const Rect& a, const Rect& b, float angleOfA,
                            float angleOfB) {
    Vec2<Cart> A[] = {Vec2<Cart>{a.x, a.y + a.h},
                      Vec2<Cart>{a.x + a.w, a.y + a.h},
                      Vec2<Cart>{a.x + a.w, a.y}, Vec2<Cart>{a.x, a.y}};
    Vec2<Cart> B[] = {Vec2<Cart>{b.x, b.y + b.h},
                      Vec2<Cart>{b.x + b.w, b.y + b.h},
                      Vec2<Cart>{b.x + b.w, b.y}, Vec2<Cart>{b.x, b.y}};

    for (auto& v : A) {
        v = Rotate(v - a.Center(), angleOfA) + a.Center();
    }

    for (auto& v : B) {
        v = Rotate(v - b.Center(), angleOfB) + b.Center();
    }

    Vec2<Cart> axes[] = {Norm(A[0] - A[1]), Norm(A[1] - A[2]),
                         Norm(B[0] - B[1]), Norm(B[1] - B[2])};

    for (auto& axis : axes) {
        float P[4];

        for (int i = 0; i < 4; ++i) P[i] = Dot(A[i], axis);

        float minA = *std::min_element(P, P + 4);
        float maxA = *std::max_element(P, P + 4);

        for (int i = 0; i < 4; ++i) P[i] = Dot(B[i], axis);

        float minB = *std::min_element(P, P + 4);
        float maxB = *std::max_element(P, P + 4);

        if (maxA < minB || minA > maxB) return false;
    }

    return true;
}
