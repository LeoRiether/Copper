#pragma once
#include <string>

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

    // TODO: would be nice if instead of string we had an enum
    virtual bool Is(const string& type) = 0;
};
