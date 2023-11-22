#pragma once

#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
#include "component/enemy/EnemyBehavior.h"
#include "math/Direction.h"

using std::unique_ptr;

class EnemyBehavior;

class RobotCan : public Component {
   public:
    //////////////////////////////
    //        Components        //
    //////////////////////////////
    float stopDistance{300};
    unique_ptr<EnemyBehavior> behavior;

    Direction direction;

    RobotCan(GameObject& associated);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);

    RobotCan& WithStopDistance(float value);
    RobotCan& WithBehavior(EnemyBehavior* value);

   private:
    constexpr static float BULLET_DELAY = 0.7f;
    Timer bulletTimer;
};
