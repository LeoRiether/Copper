#pragma once

#include "Component.h"
#include "Consts.h"
#include "Timer.h"
#include "component/enemy/RobotCan.h"

class EnemyDistancer : public Component {
   public:
    enum State {
        Roaming,
        WalkingAway,
        Shooting,
    } state{Roaming};

   private:
    RobotCan* self{nullptr};

    float walkingTime{0};
    Timer roamingTimeout{}, shotTimeout{};

    Vec2<Cart> roamingDelta;

    float& shotAnimationTime{Consts::GetFloat("robotcan.shot_animation_time")};
    float& roamingTimeConst{Consts::GetFloat("robotcan.roaming_time")};
    float& speed{Consts::GetFloat("robotcan.speed")};

    void switchState(State newState);
    void allAnimsPlay(const string& id);

   public:
    EnemyDistancer(GameObject& go);
    EnemyDistancer* WithRobotCan(RobotCan* rc);
    void Update(float dt);

    inline bool Is(CType type) { return type == CType::EnemyBehavior; }
};
