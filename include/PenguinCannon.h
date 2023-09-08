#pragma once

#include <memory>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
using std::weak_ptr;

class PenguinCannon : public Component {
   private:
    weak_ptr<GameObject> pbody;
    float angle{0};

   public:
    PenguinCannon(GameObject& associated, weak_ptr<GameObject> penguinBody);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);

    void Shoot();
};
