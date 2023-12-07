#pragma once

#include <string>

#include "Component.h"
#include "GameObject.h"
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

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::Bullet; }
    void NotifyCollision(GameObject& other);
    inline int Damage() { return damage; }
    inline bool TargetsPlayer() { return targetsPlayer; }

    Bullet* WithTargetsPlayer(bool tp);
};
