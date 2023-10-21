#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "math/Rect.h"
#include "math/Vec2.h"
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
  int renderLayer{0};
  const char *debugName;

  GameObject();
  ~GameObject();
  void Start();

  inline vector<unique_ptr<Component>> &Components() { return components; }

  void Update(float dt);
  void Render(Vec2<Cart> cameraPosition);
  bool IsDead() const;
  void RequestDelete();
  void RequestAdd(GameObject *go);
  void AddComponent(Component *cmp);
  void RemoveComponent(Component *cmp);
  Component *GetComponent(CType type) const;
  vector<Component *> GetAllComponents(CType type) const;
  void NotifyCollision(GameObject &other);
};
