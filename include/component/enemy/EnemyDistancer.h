#pragma once

#include "component/enemy/EnemyBehavior.h"

class EnemyDistancer : public EnemyBehavior {
   public:
    void Update(Enemy& enemy, float dt);
};
