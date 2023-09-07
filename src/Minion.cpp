#include "Minion.h"

#include "Bullet.h"
#include "Component.h"
#include "Game.h"
#include "GameObject.h"
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

void Minion::Shoot(Vec2 target) {
    float angle = (target - Vec2{associated.box.x, associated.box.y}).angle();
    int damage = (rng() % 10) + 5;

    auto go = new GameObject{};
    auto bullet = new Bullet{*go,    angle, 300,
                             damage, 500,   ASSETS "/img/minionbullet1.png"};
    go->box.x = associated.box.x;
    go->box.y = associated.box.y;
    go->AddComponent((Component*)bullet);
    Game::Instance().GetState().AddObject(go);
}
