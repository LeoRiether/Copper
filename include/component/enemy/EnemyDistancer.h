#pragma once

#include "Component.h"
#include "Consts.h"
#include "Timer.h"
#include "component/enemy/RobotCan.h"

class EnemyDistancer : public Component {
   public:
    enum State {
        Roaming,
        Standing,
        WalkingAway,
        Shooting,
        Pursuing,
        KeepingDistance,
    } state{Roaming};

   private:
    RobotCan* self{nullptr};

    float walkingTime{0};
    Timer roamingTimeout{}, standingStillTimeout{}, shotTimeout{},
          pursueTimeout{};

    Vec2<Cart> roamingDelta{0};

    float& shotAnimationTime{Consts::GetFloat("robotcan.shot_animation_time")};
    float& roamingTimeConst{Consts::GetFloat("robotcan.roaming_time")};
    float& standingTimeConst{Consts::GetFloat("robotcan.standing_time")};
    float& speed{Consts::GetFloat("robotcan.speed")};
    float& stopDistance{Consts::GetFloat("robotcan.stop_distance")};
    float& pursueTimeConst{Consts::GetFloat("robotcan.pursue_time")};

    void switchState(State newState);
    void animsSoftPlay(const string& id);

    bool seesPlayer();

   public:
    EnemyDistancer(GameObject& go);
    EnemyDistancer* WithRobotCan(RobotCan* rc);
    void Update(float dt);

    inline CType Key() const { return CType::EnemyBehavior; }
};
