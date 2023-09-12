#pragma once

#include <memory>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
using std::weak_ptr;

class PenguinBody : public Component {
   private:
    weak_ptr<GameObject> pcannon;
    Vec2 speed{0, 0};
    float linearSpeed{0};
    float angle{0};
    int hp{100};

   public:
    const char* DebugName() { return "PenguinBody"; }

    static PenguinBody* player;
    inline GameObject& Associated() { return associated; }

    PenguinBody(GameObject& associated);
    ~PenguinBody();

    void Start();
    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    void NotifyCollision(GameObject& other);
};
