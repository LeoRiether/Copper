#pragma once

#include "Component.h"
#include "Consts.h"
#include "GameObject.h"
#include "math/Direction.h"
#include "math/Vec2.h"

class EnemyFollower : public Component {
   private:
    const float speed{Consts::GetFloat("player.walking_speed") / 1.5f};
    const float attackDistance{63.0f};

    enum State {
        Roaming,
        Pursuing,
        Attacking,
    } state{Roaming};

    struct AttackState {
        int phase{0};
    } attackState{};

    Vec2<Cart> spawnPoint{};
    Vec2<Cart> roamingTarget{};
    Vec2<Cart> moveDelta{};
    Direction direction{NoneX, Down};

    float stunnedLevel{0};
    Vec2<Cart> knockbackVelocity{0, 0};

    void updateState(float dt);
    void updatePosition(float dt);
    void switchState(State newState);

    void animsSoftPlay(const string& id, bool loops = true);
    void animsPlay(const string& id, bool loops = true);

   public:
    EnemyFollower(GameObject& go);

    void Start();
    void Update(float dt);
    void Render(Vec2<Cart> camera);
    void NotifyCollision(GameObject& other);
    void Die();

    inline CType Key() const { return CType::EnemyBehavior; }
};
