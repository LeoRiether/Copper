#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "math/Rect.h"
#include "math/Vec2.h"
#include "physics/Tags.h"
using std::string;
using std::unique_ptr;
using std::vector;

class Component;

class GameObject {
   private:
    std::unordered_map<CType, vector<unique_ptr<Component>>> components{};
    bool isDead{false};
    bool started{false};

   public:
    Rect box{0, 0, 0, 0}, prevFrameBox{0, 0, 0, 0};
    double angle{0};
    int renderLayer{0};
    tagset tags{};
    std::weak_ptr<GameObject> weak;

    GameObject();
    ~GameObject();
    void Start();

    void Update(float dt);
    void Render(Vec2<Cart> cameraPosition);
    bool IsDead() const;
    void RequestDelete();
    void RequestAdd(GameObject* go);

    GameObject* AddComponent(Component* cmp);
    GameObject* RemoveComponent(Component* cmp);
    Component* GetComponent(CType type) const;
    vector<unique_ptr<Component>>& GetAllComponents(CType type);

    GameObject* WithFootAt(Vec2<Cart> position);
    GameObject* WithCenterAt(Vec2<Cart> position);

    void NotifyCollision(GameObject& other);
    void NotifyCollisionEnter(GameObject& other);
    void NotifyCollisionLeave(GameObject& other);
};
