#include "Minion.h"

#include "util.h"

#define MODULE "Minion"

Minion::Minion(GameObject& go, weak_ptr<GameObject> alienCenter,
               float arcOffsetRad)
    : Component(go), alienCenter(alienCenter), arc(arcOffsetRad) {}

void Minion::Update(float dt) {
    arc += dt;

    Rect centerBox = alienCenter.lock()->box;
    Vec2 center = centerBox.Center();
    Vec2 pos = center + Vec2{RADIUS, 0}.GetRotated(arc);
    associated.box.SetCenter(pos);
}

void Minion::Render(Vec2) {}

bool Minion::Is(CType type) { return type == CType::Minion; }

void Minion::Shoot(Vec2 target) {}
