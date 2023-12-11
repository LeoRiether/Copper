#pragma once

#include "Component.h"
#include "Consts.h"
#include "GameObject.h"
#include "Timer.h"
#include "math/Direction.h"

class Companion : public Component {
   public:
    enum State {
        Looking,
        Firing,
    };

    Companion(GameObject& go);

    inline CType Key() const { return CType::Companion; }

    void Update(float dt);
    void Render(Vec2<Cart> camera);
    void NotifyCollision(GameObject& other);

   private:
    float& speed{Consts::GetFloat("companion.speed")};
    float& stopDistance{Consts::GetFloat("companion.stop_distance")};
    float& firingTimeoutSecs{Consts::GetFloat("companion.firing_timeout")};

    State state{Looking};
    Timer firingTimeout{}, walkingToIdleTimeout{}, tooFarTimer{};

    Vec2<Cart> moveDelta{0, 0};
    Direction direction{NoneX, Down};

    void updatePosition(float dt);
    void updateCore(float dt);
    void updateState(float dt);

    void changeState(State newState);

    void allAnimsPlay(const string& id, bool loops = true);
    void coreAnimPlay(const string& id, bool loops = true);
    void baseAnimPlay(const string& id, bool loops = true);
};
