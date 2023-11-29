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

    Direction direction{Direction{NoneX, NoneY}};

    RobotCan(GameObject& associated);

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::RobotCan; }

    void NotifyCollision(GameObject&);

    RobotCan* WithStopDistance(float value);
};
