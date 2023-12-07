#include "component/KeepSoundAlive.h"

#include "component/Sound.h"

#define MODULE "KeepSoundAlive"

KeepSoundAlive::KeepSoundAlive(GameObject& go) : Component(go), go(go) {}
KeepSoundAlive::~KeepSoundAlive() {}

void KeepSoundAlive::Update(float) {
    auto sound = (Sound*)go.GetComponent(CType::Sound);
    if (!sound || !sound->IsPlaying()) {
        go.RequestDelete();
    }
}

void KeepSoundAlive::Render(Vec2<Cart>) {}
