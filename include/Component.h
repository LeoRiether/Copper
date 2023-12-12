#pragma once
#include <string>

#include "CType.h"
#include "GameObject.h"
#include "math/Vec2.h"
using std::string;

class GameObject;

class Component {
   protected:
   public:
    GameObject& associated;

    Component(GameObject& associated);
    virtual ~Component();
    virtual void Start();

    virtual void Update(float dt);
    virtual void Render(Vec2<Cart> camera);

    virtual CType Key() const = 0;

    virtual void NotifyCollision(GameObject& other);
    virtual void NotifyCollisionEnter(GameObject& other);
    virtual void NotifyCollisionLeave(GameObject& other);
};
