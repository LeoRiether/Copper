#pragma once

#include <cmath>

#include "math/Rect.h"
#include "math/Vec2.h"

class Collision {
   public:
    // Observação: IsColliding espera ângulos em radianos!
    // Para usar graus, forneça a sua própria implementação de Rotate,
    // ou transforme os ângulos no corpo de IsColliding.
    static bool IsColliding(const Rect& a, const Rect& b, float angleOfA,
                            float angleOfB);

   private:
    static inline float Mag(const Vec2<Cart>& p) {
        return std::sqrt(p.x * p.x + p.y * p.y);
    }

    static inline Vec2<Cart> Norm(const Vec2<Cart>& p) {
        return p * (1.f / Mag(p));
    }

    static inline float Dot(const Vec2<Cart>& a, const Vec2<Cart>& b) {
        return a.x * b.x + a.y * b.y;
    }

    static inline Vec2<Cart> Rotate(const Vec2<Cart>& p, float angle) {
        float cs = std::cos(angle), sn = std::sin(angle);
        return Vec2<Cart>{p.x * cs - p.y * sn, p.x * sn + p.y * cs};
    }
};
