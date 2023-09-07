#pragma once

#include <string>

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

class Bullet : Component {
   private:
    Vec2 speed;
    float speedNorm;
    float distanceLeft;
    int damage;

   public:
    Bullet(GameObject& associated, float angle, float speed, int damage,
           float maxDistance, const string& sprite);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    inline int Damage() { return damage; }
};
