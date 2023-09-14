#pragma once

#include <memory>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
using std::weak_ptr;

class PenguinCannon : public Component {
   private:
    weak_ptr<GameObject> pbody;
    Timer timer;

    constexpr static float SHOOT_DELAY_S = 0.3;

   public:
    const char* DebugName() { return "PenguinCannon"; }

    PenguinCannon(GameObject& associated, weak_ptr<GameObject> penguinBody);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);

    void Shoot();
};
