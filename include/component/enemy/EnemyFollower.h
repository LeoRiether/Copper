#pragma once

#include "component/enemy/EnemyBehavior.h"

class EnemyFollower : public EnemyBehavior {
   private:
   public:
    void Update(RobotCan& self, float dt);
};
