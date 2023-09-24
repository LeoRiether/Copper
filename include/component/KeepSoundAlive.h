#pragma once

#include "CType.h"
#include "Component.h"

// Keeps a GameObject alive until the Sound component stops playing
class KeepSoundAlive : public Component {
   private:
    GameObject& go;

   public:
    const char* DebugName() { return "KeepSoundAlive"; }

    KeepSoundAlive(GameObject& go);
    ~KeepSoundAlive();

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType _type);
};
