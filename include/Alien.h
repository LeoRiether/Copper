#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

using std::queue;
using std::vector;
using std::weak_ptr;

constexpr int SPEEEED = 250;

class Alien : Component {
   private:
    struct Action {
        enum ActionType { Move, Shoot };

        inline Action(ActionType type, float x, float y)
            : type(type), pos{x, y} {}

        ActionType type;
        Vec2 pos;
    };

    Vec2 speed{0, 0};
    int hp{30};

    queue<Action> tasks{};
    vector<weak_ptr<GameObject>> minions{};

   public:
    Alien(GameObject& go, int nMinions);
    ~Alien();

    void Start();
    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
};
