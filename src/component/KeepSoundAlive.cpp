#include "component/KeepSoundAlive.h"

#include "Sound.h"

#define MODULE "KeepSoundAlive"

KeepSoundAlive::KeepSoundAlive(GameObject& go) : Component(go), go(go) {}
KeepSoundAlive::~KeepSoundAlive() {}

void KeepSoundAlive::Update(float) {
    auto sound = (Sound*)go.GetComponent(CType::Sound);
    if (!sound || !sound->IsPlaying()) {
        log("requesting deletion of sound");
        go.RequestDelete();
    }
}

void KeepSoundAlive::Render(Vec2<Cart>) {}
bool KeepSoundAlive::Is(CType _type) { return _type == CType::KeepSoundAlive; }
