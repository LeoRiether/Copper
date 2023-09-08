#include "PenguinCannon.h"

#include "Component.h"
#include "Sprite.h"
#include "util.h"

PenguinCannon::PenguinCannon(GameObject& associated,
                             weak_ptr<GameObject> penguinBody)
    : Component(associated), pbody(penguinBody) {
    auto sprite = new Sprite{associated, ASSETS "/img/penguinbullet.png"};
    associated.AddComponent(sprite);
}

void PenguinCannon::Update(float dt) {}

void PenguinCannon::Render(Vec2) {}

bool PenguinCannon::Is(CType type) { return type == CType::PenguinCannon; }

void PenguinCannon::Shoot() {}
