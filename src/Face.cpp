#include "Face.h"

#include <algorithm>

#include "Sound.h"

#define MODULE "Face"

Face::Face(GameObject& associated) : Component(associated) {}

void Face::Damage(int damage) {
    hitpoints = std::max(hitpoints - damage, 0);
    if (hitpoints <= 0) {
        associated.RequestDelete();
        auto sound = (Sound*)associated.GetComponent("Sound");
        if (sound) sound->Play();
    }
}

void Face::Update(float dt) {}
void Face::Render() {}
bool Face::Is(const string& type) { return type == "Face"; }
