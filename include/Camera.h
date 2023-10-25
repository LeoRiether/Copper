#pragma once

#include "GameObject.h"
#include "math/Vec2.h"

class Camera {
   private:
    Vec2<Cart> pos{0, 0}, speed{0, 0};
    GameObject* focus = nullptr;

   public:
    inline GameObject* Focus() { return focus; }
    void Follow(GameObject* newFocus);
    void Unfollow();
    void Update(float dt);

    inline Vec2<Cart> Pos() { return pos; }
    inline void SetPos(Vec2<Cart> p) { pos = p; }
    inline Vec2<Cart> Speed() { return speed; }
    inline void SetSpeed(const Vec2<Cart> s) { speed = s; }
};
