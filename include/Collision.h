#include <algorithm>
#include <cmath>

#include "Rect.h"
#include "Vec2.h"

class Collision {
   public:
    // Observação: IsColliding espera ângulos em radianos!
    // Para usar graus, forneça a sua própria implementação de Rotate,
    // ou transforme os ângulos no corpo de IsColliding.
    static bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB);

   private:
    static inline float Mag(const Vec2& p) {
        return std::sqrt(p.x * p.x + p.y * p.y);
    }

    static inline Vec2 Norm(const Vec2& p) { return p * (1.f / Mag(p)); }

    static inline float Dot(const Vec2& a, const Vec2& b) {
        return a.x * b.x + a.y * b.y;
    }

    static inline Vec2 Rotate(const Vec2& p, float angle) {
        float cs = std::cos(angle), sn = std::sin(angle);
        return Vec2{p.x * cs - p.y * sn, p.x * sn + p.y * cs};
    }
};

// Aqui estão três operadores que sua classe Vec2 deve precisar, se já não
// tiver. Se sua classe tiver métodos para Mag, Norm, Dot e/ou Rotate, você pode
// substituir os usos desses métodos por usos dos seus, mas garanta que deem
// resultados corretos.

// Vec2 operator+(const Vec2& rhs) const {
//    return Vec2(x + rhs.x, y + rhs.y);
// }

// Vec2 operator-(const Vec2& rhs) const {
//    return Vec2(x - rhs.x, y - rhs.y);
// }

// Vec2 operator*(const float rhs) const {
//    return Vec2(x * rhs, y * rhs);
// }
