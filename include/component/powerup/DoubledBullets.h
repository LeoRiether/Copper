#pragma once

#include "Component.h"
#include "Timer.h"

class DoubledBullets : public Component {
   public:
    DoubledBullets(GameObject& go);

    void Update(float dt);

    inline CType Key() const { return CType::DoubledBullets; }

   private:
    constexpr static float timeAlive{10.0f};
    Timer timeout{};

    bool started{false};
    void start();
};
