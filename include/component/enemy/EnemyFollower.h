#pragma once

#include "Component.h"
#include "Consts.h"
#include "GameObject.h"
#include "component/enemy/RobotCan.h"
#include "math/Vec2.h"

class EnemyFollower : public Component {
   private:
    RobotCan* self;

    float& speed{Consts::GetFloat("robotcan.speed")};

    Vec2<Cart> moveDelta;

   public:
    EnemyFollower(GameObject& go);
    EnemyFollower* WithRobotCan(RobotCan* self);

    void Update(float dt);
    void Render(Vec2<Cart> camera);

    inline CType Key() const { return CType::EnemyBehavior; }
};
