#pragma once

#include <string>

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
#include "math/Vec2.h"
using std::string;

class Bullet : public Component {
   private:
    Vec2<Cart> speed;
    float speedNorm;
    float distanceLeft;
    int damage;
    bool targetsPlayer;

   public:
    Bullet(GameObject& associated, float speed, float angle, int damage,
           float maxDistance, bool targetsPlayer);

    static GameObject* Make(Vec2<Cart> center, float angle);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);
    void NotifyCollision(GameObject& other);
    inline int Damage() { return damage; }
    inline bool TargetsPlayer() { return targetsPlayer; }
};
