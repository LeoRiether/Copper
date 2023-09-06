#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "Rect.h"
#include "Vec2.h"
using std::string;
using std::unique_ptr;
using std::vector;

class Component;

class GameObject {
   private:
    // TODO: maybe a map<ComponentType, shared_ptr<Component>> would be better
    // here
    vector<unique_ptr<Component>> components;
    bool isDead;
    bool started;

   public:
    Rect box;

    GameObject();
    ~GameObject();
    void Start();

    void Update(float dt);
    void Render(Vec2 cameraPosition);
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cmp);
    void RemoveComponent(Component* cmp);
    Component* GetComponent(CType type);
};
