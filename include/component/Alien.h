#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "Minion.h"
#include "Timer.h"
#include "Vec2.h"

using std::queue;
using std::vector;
using std::weak_ptr;

constexpr int SPEEEED = 250;

class Alien : public Component {
   private:
    enum AlienState { Resting, Moving };
    constexpr static float REST_TIME_S = 1.5f;

    Vec2 speed{0, 0};
    int hp{30};

    AlienState state{Resting};
    Timer restTimer;
    Vec2 destination;

    vector<weak_ptr<GameObject>> minions{};

    void BulletHell();
    Minion* ClosestMinion(Vec2 target);

   public:
    static int alienCount;

    Alien(GameObject& go, int nMinions);
    ~Alien();

    void Start();
    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    void NotifyCollision(GameObject& other);
};
