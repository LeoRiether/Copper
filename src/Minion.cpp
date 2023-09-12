#include "Minion.h"

#include <random>

#include "Bullet.h"
#include "CType.h"
#include "Collider.h"
#include "Component.h"
#include "Game.h"
#include "GameObject.h"
#include "util.h"

#define MODULE "Minion"

Minion::Minion(GameObject& go, weak_ptr<GameObject> alienCenter,
               float arcOffsetRad)
    : Component(go), alienCenter(alienCenter), arc(arcOffsetRad) {
    associated.debugName = "Minion";
    std::uniform_real_distribution<float> dist(1, 1.5);
    auto sprite = (Sprite*)go.GetComponent(CType::Sprite);
    float scale = dist(rng);
    sprite->SetScale(scale, scale);
    go.AddComponent(new Collider{go, Vec2{scale, scale}});
}

void Minion::Update(float dt) {
    if (alienCenter.expired()) {
        associated.RequestDelete();
        return;
    }

    arc += dt;
    associated.angle = arc;

    Rect centerBox = alienCenter.lock()->box;
    Vec2 center = centerBox.Center();
    Vec2 pos = center + Vec2{RADIUS, 0}.GetRotated(arc);
    associated.box.SetCenter(pos);
}

void Minion::Render(Vec2) {}

bool Minion::Is(CType type) { return type == CType::Minion; }

void Minion::Shoot(Vec2 target) {
    float angle = (target - associated.box.Center()).angle();
    int damage = (rng() % 10) + 5;

    auto go = new GameObject{};
    auto bullet = new Bullet{
        *go, angle, 300, damage, 500, 3, ASSETS "/img/minionbullet2.png", true};
    go->box.x = associated.box.Center().x;
    go->box.y = associated.box.Center().y;
    go->AddComponent(bullet);
    associated.RequestAdd(go);
}
