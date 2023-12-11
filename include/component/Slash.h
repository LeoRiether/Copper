#pragma once

#include "Component.h"
#include "GameObject.h"

class Slash : public Component {
   public:
    Slash(GameObject& go, float angle);

    inline CType Key() const { return CType::Slash; }

    void Update(float dt); 

   private:
    Vec2<Cart> speed{};
};
