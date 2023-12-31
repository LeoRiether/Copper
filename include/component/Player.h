#pragma once

#include <memory>
#include <optional>

#include "Powerups.h"
#include "CType.h"
#include "Component.h"
#include "Consts.h"
#include "GameData.h"
#include "GameObject.h"
#include "Timer.h"
#include "math/Direction.h"
#include "math/Vec2.h"
using std::weak_ptr;

constexpr float DASH_DURATION = 0.3;
constexpr float DASH_TIMEOUT = 0.2;

struct DashState {
    Timer timeSinceStart{};
    Timer timeout{};
    bool queuedAttack{false};
};

struct AttackState {
    int phase{0};
    bool queuedAttack{false};
    bool queuedDash{false};
    bool colliderDeployed{false};
};

class Player : public Component {
   public:
    enum State {
        Idle,
        Walking,
        Dashing,
        Attacking,
        StageTransition,
    };

    vector<Vec2<Iso>> Trail{};

    int hp{GameData::playerHp};
    int hpLoss{0};
    Timer hpLossTimer{};

    Vec2<Cart> moveVec{};

   private:
    float& walkingSpeed{Consts::GetFloat("player.walking_speed")};
    float& stepsTiming{Consts::GetFloat("player.steps_timing")};

    Direction direction{NoneX, Down};
    DashState dashState{};
    AttackState attackState{};
    Timer stepsTimer{};
    Timer trailTimer{};

    Vec2<Cart> knockbackVelocity{0, 0};

    void UpdateState(float dt);
    void UpdatePosition(float dt);
    void ConstrainToTile();

    bool dead{false};

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
    void NotifyCollisionEnter(GameObject& other);
    void RequestDelete();

    /* Finds the player if in view, or a point in the trail that's visible */
    std::optional<Vec2<Iso>> LookForMe(Rect isoViewpoint);

    /* Transitions the state from the current to `newState` */
    void ChangeState(State newState);
    /* Only calls ChangeState if state != newState */
    void MaybeChangeState(State newState);


    Powerups powerups{};
    void AddPowerup(Powerups::Kind);
    void RemovePowerup(Powerups::Kind);
};
