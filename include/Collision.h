#include <algorithm>
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

// Aqui estão três operadores que sua classe Vec2<Cart> deve precisar, se já não
// tiver. Se sua classe tiver métodos para Mag, Norm, Dot e/ou Rotate, você pode
// substituir os usos desses métodos por usos dos seus, mas garanta que deem
// resultados corretos.

// Vec2<Cart> operator+(const Vec2<Cart>& rhs) const {
//    return Vec2<Cart>(x + rhs.x, y + rhs.y);
// }

// Vec2<Cart> operator-(const Vec2<Cart>& rhs) const {
//    return Vec2<Cart>(x - rhs.x, y - rhs.y);
// }

// Vec2<Cart> operator*(const float rhs) const {
//    return Vec2<Cart>(x * rhs, y * rhs);
// }
