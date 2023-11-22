#pragma once

#include "component/enemy/RobotCan.h"

class RobotCan;

class EnemyBehavior {
   public:
    virtual ~EnemyBehavior() {}
    virtual void Update(RobotCan&, float) {}
};
