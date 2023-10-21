#pragma once

#include <memory>

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
  };

private:
  float &walkingSpeed{Consts::GetFloat("player.walking_speed")};
  Direction direction;
  DashState dashState;

  /* Transitions the state from the current to `newState` */
  void ChangeState(State newState);
  /* Only calls ChangeState if state != newState */
  void MaybeChangeState(State newState);

  void UpdateState();
  void UpdatePosition(float dt);
  void ConstrainToTile();

public:
  static Player *player;
  inline GameObject &Associated() { return associated; }
  Player(GameObject &associated);
  ~Player();

  State state{Idle};

  void Start();
  void Update(float dt);
  void Render(Vec2<Cart> camera);
  bool Is(CType type);
  void NotifyCollision(GameObject &other);
  void RequestDelete();
};
