#include "component/Pickupable.h"

#include <cmath>

#include "physics/Tags.h"

#define MODULE "Pickupable"

Pickupable::Pickupable(GameObject& go) : Component(go) {}

void Pickupable::Update(float dt) {
    t += 3.0f * dt;
    associated.box.OffsetBy({0, sinf(t) * 20.f * dt});
}

void Pickupable::NotifyCollisionEnter(GameObject& other) {
    if (other.tags.test(tag::Player)) {
        associated.RemoveComponent(this);
    }
}
