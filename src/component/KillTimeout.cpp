#include "component/KillTimeout.h"

#define MODULE "KillTimeout"

KillTimeout::KillTimeout(GameObject& go, float t) : Component(go), timeout(t) {}

void KillTimeout::Update(float dt) {
    timeout -= dt;
    if (timeout <= 0) associated.RequestDelete();
}
