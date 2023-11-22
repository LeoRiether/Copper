#pragma once

#include "Component.h"
#include "Consts.h"
#include "Timer.h"
#include "component/enemy/RobotCan.h"

class EnemyDistancer : public Component {
   public:
    enum State {
        WalkingAway,
        Shooting,
    } state;

   private:
    RobotCan* self{nullptr};

    float walkingTime{0};
    Timer shotTimeout{}, timeBetweenShots{};

    float& bulletDelay{Consts::GetFloat("robotcan.bullet_delay")};
    float& timeBetweenShotsConst{
        Consts::GetFloat("robotcan.time_between_shots")};

    void switchState(State newState);
    void allAnimsPlay(const string& id);

   public:
    EnemyDistancer(GameObject& go);
    EnemyDistancer* WithRobotCan(RobotCan* rc);
    void Update(float dt);

    inline bool Is(CType type) { return type == CType::EnemyBehavior; }
};
