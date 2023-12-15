#pragma once

#include "Component.h"
#include "Timer.h"

class StrongerAttack : public Component {
   public:
    StrongerAttack(GameObject& go);

    void Update(float dt);

    inline CType Key() const { return CType::StrongerAttack; }

   private:
    constexpr static float timeAlive{15.0f};
    Timer timeout{};

    bool started{false};
    void start();
};
