#pragma once

#include "Component.h"
#include "Timer.h"

class Speedy : public Component {
   public:
    Speedy(GameObject& go);

    void Update(float dt);

    inline CType Key() const { return CType::Speedy; }

   private:
    constexpr static float timeAlive{10.0f};
    Timer timeout{};

    bool started{false};
    void start();
};
