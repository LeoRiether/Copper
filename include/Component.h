#pragma once
#include <string>

#include "GameObject.h"
using std::string;

class GameObject;

class Component {
   private:
    GameObject& associated;

   public:
    Component(GameObject& associated);
    virtual ~Component() = 0;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool Is(const string& type) = 0;
};
