#pragma once

#include <memory>
#include <optional>

#include "CType.h"
#include "Component.h"
#include "Consts.h"
#include "GameObject.h"
#include "Timer.h"
#include "math/Direction.h"
#include "math/Vec2.h"
using std::weak_ptr;

constexpr float DASH_DURATION = 0.3;
constexpr float DASH_TIMEOUT = 0.2;

struct DashState {
    Timer timeSinceStart;
    Timer timeout;
};

class Player : public Component {
   public:
    enum State {
        Idle,
        Walking,
        Dashing,
        StageTransition,
    };

    vector<Vec2<Iso>> Trail{};

    int hp;
    int hpLoss;

   private:
    float& walkingSpeed{Consts::GetFloat("player.walking_speed")};
    float& stepsTiming{Consts::GetFloat("player.steps_timing")};

    Direction direction{NoneX, Down};
    DashState dashState;
    Timer stepsTimer{};
    Timer trailTimer{};

    Timer hpLossTimer{};

    float flashTimeout{0};
    Vec2<Cart> knockbackVelocity{0, 0};

    void UpdateState(float dt);
    void UpdatePosition(float dt);
    void ConstrainToTile();

   public:
    static Player* player;
    inline GameObject& Associated() { return associated; }
    Player(GameObject& associated);
    ~Player();

    State state{Idle};

    void Start();
    void Update(float dt);
    void Render(Vec2<Cart> camera);
    inline CType Key() const { return CType::Player; }
    void NotifyCollision(GameObject& other);
    void RequestDelete();

    /* Finds the player if in view, or a point in the trail that's visible */
    std::optional<Vec2<Iso>> LookForMe(Rect isoViewpoint);

    /* Transitions the state from the current to `newState` */
    void ChangeState(State newState);
    /* Only calls ChangeState if state != newState */
    void MaybeChangeState(State newState);
};