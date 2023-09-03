#pragma once
#include <string>
#include <vector>

#include "Component.h"
#include "Rect.h"
using std::string;
using std::vector;

class Component;

class GameObject {
   public:
    Rect box;

    GameObject();
    ~GameObject();

    void Update(float dt);
    void Render();
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cmp);
    void RemoveComponent(Component* cmp);
    Component* GetComponent(const string& type);

   private:
    vector<Component*> components;
    bool isDead;
};
