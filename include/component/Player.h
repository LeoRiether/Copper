#pragma once

#include <memory>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "math/Direction.h"
#include "math/Vec2.h"
using std::weak_ptr;

class Player : public Component {
   private:
    Direction lastDirection;

    void UpdatePosition(float dt);
    void ConstrainToTile();

   public:
    const char* DebugName() { return "Player"; }

    static Player* player;
    inline GameObject& Associated() { return associated; }

    Player(GameObject& associated);
    ~Player();

    void Start();
    void Update(float dt);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);
    void NotifyCollision(GameObject& other);
    void RequestDelete();
};
