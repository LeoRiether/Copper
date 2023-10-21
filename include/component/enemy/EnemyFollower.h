#pragma once

#include "component/enemy/EnemyBehavior.h"

class EnemyFollower : public EnemyBehavior {
private:
public:
  void Update(Enemy &self, float dt);
};
