#include "component/Slash.h"

#include "Component.h"

#define MODULE "Slash"

Slash::Slash(GameObject& go, float angle)
    : Component(go), speed(Vec2<Cart>{140, 0}.GetRotated(angle)) {}

void Slash::Update(float dt) { associated.box.OffsetBy(speed * dt); }
