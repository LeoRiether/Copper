#pragma once

#include "Component.h"
#include "Consts.h"
#include "GameObject.h"
#include "component/enemy/RobotCan.h"

class EnemyFollower : public Component {
   private:
    RobotCan* self;

    float& speed{Consts::GetFloat("robotcan.speed")};

   public:
    EnemyFollower(GameObject& go);
    EnemyFollower* WithRobotCan(RobotCan* self);
    void Update(float dt);

    inline bool Is(CType type) { return type == CType::EnemyBehavior; }
};
