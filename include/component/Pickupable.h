#pragma once

#include "Component.h"

class Pickupable : public Component {
   public:
    Pickupable(GameObject& go);

    void Update(float dt);
    void NotifyCollisionEnter(GameObject& other);

    inline CType Key() const { return CType::Pickupable; }

   private:
    float t{0};
};
