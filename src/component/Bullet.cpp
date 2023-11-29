#include "component/Bullet.h"

#include <cmath>

#include "CType.h"

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

void Bullet::Update(float dt) {
    associated.box.OffsetBy(speed * dt);
    distanceLeft -= speedNorm * dt;
    if (distanceLeft <= 0) associated.RequestDelete();
}

void Bullet::Render(Vec2<Cart>) {}

void Bullet::NotifyCollision(GameObject& other) {
    if (targetsPlayer && other.GetComponent(CType::Player))
        associated.RequestDelete();
    if (!targetsPlayer &&
        (other.GetComponent(CType::Alien) || other.GetComponent(CType::Minion)))
        associated.RequestDelete();
}
