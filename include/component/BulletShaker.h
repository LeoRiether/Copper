#pragma once

#include "Component.h"
#include "GameObject.h"

class BulletShaker : public Component {
   public:
    BulletShaker(GameObject& go);

    void Update(float dt);
    void NotifyCollision(GameObject& other);

    inline CType Key() const { return CType::BulletShaker; }

   private:
    float trauma;
    Vec2<Cart> offset{0, 0};
};
