#pragma once

#include <memory>

#include "GameObject.h"
#include "Timer.h"
#include "math/Vec2.h"

class Camera {
   private:
    Vec2<Cart> pos{0, 0}, speed{0, 0};
    GameObject* focus{nullptr};

    struct {
        bool exists{false};
        std::weak_ptr<GameObject> go{};
    } secFocus;

    Timer focusChanged{};

    Vec2<Cart> shakeDelta{0, 0};

    void updateFocus(float dt);
    void updateFromInput(float dt);
    void updateShake();

   public:
    inline GameObject* Focus() { return focus; }
    void Follow(GameObject* newFocus);
    void SecondaryFollow(std::weak_ptr<GameObject> go);
    void Unfollow();
    void SecondaryUnfollow();
    void Update(float dt);

    Vec2<Cart> Pos();
    void SetPos(Vec2<Cart> p);
    inline Vec2<Cart> Speed() { return speed; }
    inline void SetSpeed(const Vec2<Cart> s) { speed = s; }
};
