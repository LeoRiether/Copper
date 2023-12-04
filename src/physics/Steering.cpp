#include "physics/Steering.h"

#include <cmath>

#include "physics/CollisionEngine.h"
#include "util.h"

#define MODULE "Steering"

Steering* Steering::Add(int i, float w) {
    values[i] += w;
    return this;
}

Steering* Steering::AddTerrain(Vec2<Iso> pos) {
    for (int i = 0; i < 8; i++) {
        for (int prox = 120; prox >= 60; prox -= 20) {
            auto endpoint = pos + direction(i) * prox;
            if (CollisionEngine::TerrainContainsSegment(pos, endpoint)) {
                values[(i + 4) % 8] += 60;
            } else {
                break;
            }
        }
    }
    return this;
}

Vec2<Iso> Steering::Result() const {
    Vec2<Iso> res{0, 0};
    for (int i = 0; i < 8; i++) {
        res = res + direction(i) * values[i];
    }
    return res;
}

Vec2<Iso> Steering::direction(int i) const {
    return Vec2<Iso>{1, 0}.GetRotated(i * PI / 4.0f);
}
