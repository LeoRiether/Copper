#include "Face.h"

#include <algorithm>

#include "InputManager.h"
#include "KeepSoundAlive.h"
#include "Sound.h"
#include "util.h"

#define MODULE "Face"

Face::Face(GameObject& associated) : Component(associated) {}
Face::~Face() {}

void Face::Damage(int damage) {
    hitpoints = std::max(hitpoints - damage, 0);
    if (hitpoints <= 0) {
        auto sound = (Sound*)associated.GetComponent(CType::Sound);
        if (sound) {
            sound->Play();
            auto keepalive = new KeepSoundAlive(associated);
            associated.AddComponent((Component*)keepalive);
            associated.RemoveComponent(associated.GetComponent(CType::Sprite));
            associated.box = Rect{0, 0, 0, 0};
        } else {
            associated.RequestDelete();
        }
    }
}

void Face::Update(float) {
    auto& input = InputManager::Instance();

    if (input.MousePress(1) && associated.box.Contains(
            Vec2{(float)input.MouseX(), (float)input.MouseY()})) {
        Damage(rng() % 10 + 10);
    }
}

void Face::Render() {}
bool Face::Is(CType type) { return type == CType::Face; }
