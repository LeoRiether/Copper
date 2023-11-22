#include "component/Bullet.h"

#include <cmath>
#include <iterator>

#include "CType.h"
#include "component/Animation.h"
#include "component/Collider.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "Bullet"

Bullet::Bullet(GameObject& associated, float speed, float angle, int damage,
               float maxDistance, bool targetsPlayer)
    : Component(associated),
      speed{float(speed * cos(angle)), float(speed * sin(angle))},
      speedNorm(this->speed.norm()),
      distanceLeft(maxDistance),
      damage(damage),
      targetsPlayer(targetsPlayer) {
    associated.angle = angle;
}

GameObject* Bullet::Make(Vec2<Cart> center, float angle) {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/laser-bullet.png"};
    go->AddComponent(new Bullet{*go, 700, angle, 20, 1000, true});
    go->AddComponent(sprite);
    go->AddComponent(
        (new Collider{*go})->WithBase(Rect{46.555, 55.1247, 28.3535, 11.9118}));
    go->box =
        Rect{0, 0, (float)sprite->SheetWidth(), (float)sprite->SheetHeight()};
    go->box.SetCenter(center);
    return go;
}

void Bullet::Update(float dt) {
    associated.box.x += speed.x * dt;
    associated.box.y += speed.y * dt;
    distanceLeft -= speedNorm * dt;
    if (distanceLeft <= 0) associated.RequestDelete();
}

void Bullet::Render(Vec2<Cart>) {}

bool Bullet::Is(CType type) { return type == CType::Bullet; }

void Bullet::NotifyCollision(GameObject& other) {
    if (targetsPlayer && other.GetComponent(CType::Player))
        associated.RequestDelete();
    if (!targetsPlayer &&
        (other.GetComponent(CType::Alien) || other.GetComponent(CType::Minion)))
        associated.RequestDelete();
}
