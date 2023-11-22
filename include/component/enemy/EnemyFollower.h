#pragma once

#include "Component.h"
#include "GameObject.h"
#include "component/enemy/RobotCan.h"

class EnemyFollower : public Component {
   private:
    RobotCan* self;

   public:
    EnemyFollower(GameObject& go);
    EnemyFollower* WithRobotCan(RobotCan* self);
    void Update(float dt);

    inline bool Is(CType type) { return type == CType::EnemyBehavior; }
};
