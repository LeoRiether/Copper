#include "Bullet.h"

#include <math.h>

#include <iterator>

#include "CType.h"
#include "Sprite.h"

#define MODULE "Bullet"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage,
               float maxDistance, const string& spritePath)
    : Component(associated),
      speed{speed * cos(angle), speed * sin(angle)},
      speedNorm(this->speed.norm()),
      distanceLeft(maxDistance),
      damage(damage) {
    auto sprite = new Sprite{associated, spritePath};
    associated.AddComponent((Component*)sprite);
}

void Bullet::Update(float dt) {
    associated.box.x += speed.x * dt;
    associated.box.y += speed.y * dt;
    distanceLeft -= speedNorm * dt;
    if (distanceLeft <= 0) associated.RequestDelete();
}

void Bullet::Render(Vec2) {}

bool Bullet::Is(CType type) { return type == CType::Bullet; }
