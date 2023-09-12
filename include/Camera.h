#pragma once

#include "GameObject.h"
#include "Vec2.h"

class Camera {
   private:
    Vec2 pos{0, 0}, speed{0, 0};
    GameObject* focus = nullptr;

   public:
    inline GameObject* Focus() { return focus; }
    void Follow(GameObject* newFocus);
    void Unfollow();
    void Update(float dt);

    inline Vec2 Pos() { return pos; }
    inline Vec2 Speed() { return speed; }
    inline void SetSpeed(const Vec2 s) { speed = s; }
};
