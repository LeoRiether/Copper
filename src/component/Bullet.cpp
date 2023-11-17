#include "component/Bullet.h"

#include <cmath>
#include <iterator>

#include "CType.h"
#include "component/Animation.h"
#include "component/Collider.h"
#include "component/Sprite.h"

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
