#pragma once
#include <string>

#include "CType.h"
#include "GameObject.h"
#include "math/Vec2.h"
using std::string;

class GameObject;

class Component {
   protected:
    GameObject& associated;

   public:
    Component(GameObject& associated);
    virtual ~Component();
    virtual void Start();
    virtual const char* DebugName() { return "<unnamed>"; }

    virtual void Update(float dt) = 0;
    virtual void Render(Vec2 camera) = 0;

    virtual bool Is(CType type) = 0;

    virtual void NotifyCollision(GameObject& other);
};
