#pragma once

#include "GameObject.h"
#include "math/Vec2.h"

class Camera {
   private:
    Vec2<Cart> pos{0, 0}, speed{0, 0};
    GameObject* focus{nullptr};

    Vec2<Cart> shakeDelta{0, 0};

   public:
    inline GameObject* Focus() { return focus; }
    void Follow(GameObject* newFocus);
    void Unfollow();
    void Update(float dt);

    Vec2<Cart> Pos();
    void SetPos(Vec2<Cart> p);
    inline Vec2<Cart> Speed() { return speed; }
    inline void SetSpeed(const Vec2<Cart> s) { speed = s; }
};
