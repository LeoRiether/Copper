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
    weak_ptr<GameObject> tileMap;
    Vec2 speed{0, 0};
    float linearSpeed{0};
    float angle{0};
    int hp{100};

    void ReflectOnMapBorder();

   public:
    const char* DebugName() { return "PenguinBody"; }

    static PenguinBody* player;
    inline GameObject& Associated() { return associated; }

    PenguinBody(GameObject& associated, weak_ptr<GameObject> tileMap);
    ~PenguinBody();

    void Start();
    void Update(float dt);
    void Render(Vec2 camera);
    bool Is(CType type);
    void NotifyCollision(GameObject& other);
    void RequestDelete();
};
