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
    bool isDead{false};
    bool started{false};

   public:
    Rect box;
    double angle{0};
    const char* debugName;

    GameObject();
    ~GameObject();
    void Start();

    void Update(float dt);
    void Render(Vec2 cameraPosition);
    bool IsDead();
    void RequestDelete();
    void RequestAdd(GameObject* go);
    void AddComponent(Component* cmp);
    void RemoveComponent(Component* cmp);
    Component* GetComponent(CType type);
};
