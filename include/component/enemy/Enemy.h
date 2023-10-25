#pragma once

#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
#include "component/enemy/EnemyBehavior.h"
#include "math/Direction.h"

using std::unique_ptr;

class EnemyBehavior;

class Enemy : public Component {
   public:
    //////////////////////////////
    //        Components        //
    //////////////////////////////
    float stopDistance{300};
    unique_ptr<EnemyBehavior> behavior;

    Direction direction;

    Enemy(GameObject& associated);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);

    Enemy& WithStopDistance(float value);
    Enemy& WithBehavior(EnemyBehavior* value);

   private:
    constexpr static float BULLET_DELAY = 0.7f;
    Timer bulletTimer;
};
