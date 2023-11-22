#pragma once

#include "component/enemy/EnemyBehavior.h"

class EnemyNop : public EnemyBehavior {
   private:
   public:
    void Update(RobotCan& self, float dt);
};
