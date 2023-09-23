#pragma once

#include <string>

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
using std::string;

class Bullet : public Component {
   private:
    Vec2 speed;
    float speedNorm;
    float distanceLeft;
    int damage;
    bool targetsPlayer;

   public:
    const char* DebugName() { return "Bullet"; }

    Bullet(GameObject& associated, float angle, float speed, int damage,
           float maxDistance, int spriteFrameCount, const string& sprite,
           bool targetsPlayer);

    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    virtual void NotifyCollision(GameObject& other);
    inline int Damage() { return damage; }
    inline bool TargetsPlayer() { return targetsPlayer; }
};