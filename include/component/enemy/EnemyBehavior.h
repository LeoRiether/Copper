#pragma once

#include "component/enemy/Enemy.h"

class Enemy;

class EnemyBehavior {
   public:
    virtual ~EnemyBehavior() {}
    virtual void Update(Enemy&, float) {}
};
