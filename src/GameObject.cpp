#include "GameObject.h"

#include <algorithm>
#include <memory>

#include "Component.h"
#include "Game.h"
#include "component/Collider.h"
#include "component/TileMap.h"
#include "util.h"

#define MODULE "GameObject"

GameObject::GameObject() {}
GameObject::~GameObject() {
    components
        .clear();  // pretty sure it would be cleared without this line anyway
}

void GameObject::Start() {
    if (started) return;
    for (auto& component : components) {
        component->Start();
    }
}

void GameObject::Update(float dt) {
    Collider* collider = nullptr;
    for (auto& component : components) {
        component->Update(dt);
        if (component->Is(CType::Collider))
            collider = (Collider*)component.get();
    }
    if (collider) collider->Update(dt);
}

void GameObject::Render(Vec2<Cart> camera) {
    for (auto& component : components) {
        component->Render(camera);
    }
}

bool GameObject::IsDead() { return isDead; }

void GameObject::RequestDelete() { isDead = true; }
void GameObject::RequestAdd(GameObject* go) {
    Game::Instance().GetState().RequestAddObject(go);
}

void GameObject::AddComponent(Component* cmp) { components.emplace_back(cmp); }

// NOTE: components are removed without maintaining order
void GameObject::RemoveComponent(Component* cmp) {
    for (size_t i = 0; i < components.size(); i++) {
        if (components[i].get() == cmp) {
            std::swap(components[i], components.back());
            components.pop_back();
            return;
        }
    }
}

// Should return an std::optional<Component*>, really
Component* GameObject::GetComponent(CType type) {
    for (const auto& component : components) {
        if (component->Is(type)) return component.get();
    }
    return nullptr;
}

void GameObject::NotifyCollision(GameObject& other) {
    for (auto& component : components) {
        component->NotifyCollision(other);
    }
}
