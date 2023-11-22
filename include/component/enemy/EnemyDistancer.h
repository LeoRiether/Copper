#pragma once

#include "Timer.h"
#include "component/enemy/EnemyBehavior.h"

class EnemyDistancer : public EnemyBehavior {
   private:
    float walkingTime{0};

   public:
    void Update(RobotCan& enemy, float dt);
};
