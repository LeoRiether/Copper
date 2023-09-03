#pragma once
#include <string>

#include "CType.h"
#include "GameObject.h"
using std::string;

class GameObject;

class Component {
   protected:
    GameObject& associated;

   public:
    Component(GameObject& associated);
    virtual ~Component();

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    virtual bool Is(CType type) = 0;
};
