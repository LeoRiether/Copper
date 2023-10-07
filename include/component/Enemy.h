#pragma once

#include "Component.h"
#include "GameObject.h"
#include "math/Direction.h"

class Enemy : public Component {
   private:
    Direction direction;

   public:
    Enemy(GameObject& associated);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);
};
