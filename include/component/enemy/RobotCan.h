#pragma once

#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "Timer.h"
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
    bool Is(CType type);

    RobotCan* WithStopDistance(float value);
};
