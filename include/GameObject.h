#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "Rect.h"
using std::unique_ptr;
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
    // TODO: maybe a map<ComponentType, shared_ptr<Component>> would be better
    // here
    vector<unique_ptr<Component>> components;
    bool isDead;
};
