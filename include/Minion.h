#pragma once

#include <memory>

#include "GameObject.h"

using std::weak_ptr;

class Minion : public Component {
   private:
    static constexpr float RADIUS = 150;

    weak_ptr<GameObject> alienCenter;
    float arc;

   public:
    const char* DebugName() { return "Minion"; }

    Minion(GameObject& go, weak_ptr<GameObject> alienCenter,
           float arcOffsetRad = 0);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    void Shoot(Vec2 target);
};
