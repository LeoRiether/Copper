#include "component/BulletShaker.h"

#include "Prefabs.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "BulletShaker"

BulletShaker::BulletShaker(GameObject& go) : Component(go) {}

void BulletShaker::Update(float) {
    if (trauma >= 0.01) {
        trauma *= 0.9;

        associated.box.OffsetBy(offset * -1);
        auto delta = [&]() { return 50.0f * trauma * trauma * randf(-1, 1); };
        offset = {delta(), delta()};
        associated.box.OffsetBy(offset);
    } else {
        // undo shake
        associated.box.OffsetBy(offset * -1);
        offset = {0, 0};
    }
}

void BulletShaker::NotifyCollision(GameObject& other) {
    if (other.tags.test(tag::Bullet)) {
        // Add trauma
        trauma = std::min(trauma + 0.4, 1.0);

        // Explosion
        auto hitpoint = other.box.Center();
        hitpoint = hitpoint + Vec2<Cart>{25, 0}.GetRotated(other.angle);
        associated.RequestAdd(MakeExplosion1()->WithCenterAt(hitpoint));

        other.RequestDelete();
    }
}
