#pragma once

#include "Component.h"
#include "GameObject.h"

class HitMarker : public Component {
   public:
    HitMarker(GameObject& go, int dmg);

    void Update(float dt);

    inline CType Key() const { return CType::HitMarker; }

   private:
    static constexpr float g = 1000;
    Vec2<Cart> speed{80, -400};
    float alpha{255};
};
