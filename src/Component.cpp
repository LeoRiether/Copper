#include "Component.h"

#define MODULE "Component"

Component::Component(GameObject &associated) : associated(associated) {}

Component::~Component() {}

void Component::Start() {}
void Component::Update(float) {}
void Component::Render(Vec2<Cart>) {}

void Component::NotifyCollision(GameObject &) {}
