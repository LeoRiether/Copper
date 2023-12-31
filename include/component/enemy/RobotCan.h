#pragma once

#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "math/Direction.h"

using std::unique_ptr;

class EnemyBehavior;

class RobotCan : public Component {
   public:
    //////////////////////////////
    //        Components        //
    //////////////////////////////
    float stopDistance{300};
    Component* behavior;

    float stunnedLevel{0};

    Direction direction{NoneX, Down};

    RobotCan(GameObject& associated);

    void Start();
    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::RobotCan; }

    void NotifyCollisionEnter(GameObject&);

    RobotCan* WithStopDistance(float value);
    RobotCan* WithHp(int hp);

    void Die();

   private:
    Vec2<Cart> knockbackVelocity{0, 0};
};
