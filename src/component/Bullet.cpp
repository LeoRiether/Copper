#include "component/Bullet.h"

#include <math.h>

#include <iterator>

#include "CType.h"
#include "Collider.h"
#include "component/Sprite.h"

#define MODULE "Bullet"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage,
               float maxDistance, int spriteFrameCount,
               const string& spritePath, bool targetsPlayer)
    : Component(associated),
      speed{speed * cos(angle), speed * sin(angle)},
      speedNorm(this->speed.norm()),
      distanceLeft(maxDistance),
      damage(damage),
      targetsPlayer(targetsPlayer) {
    auto sprite = new Sprite{associated, spritePath, spriteFrameCount, 0.1};
    associated.AddComponent(sprite);
    associated.angle = angle;

    associated.AddComponent(new Collider{associated});
}

void Bullet::Update(float dt) {
    associated.box.x += speed.x * dt;
    associated.box.y += speed.y * dt;
    distanceLeft -= speedNorm * dt;
    if (distanceLeft <= 0) associated.RequestDelete();
}

void Bullet::Render(Vec2) {}

bool Bullet::Is(CType type) { return type == CType::Bullet; }

void Bullet::NotifyCollision(GameObject& other) {
    if (targetsPlayer && other.GetComponent(CType::PenguinBody))
        associated.RequestDelete();
    if (!targetsPlayer &&
        (other.GetComponent(CType::Alien) || other.GetComponent(CType::Minion)))
        associated.RequestDelete();
}
