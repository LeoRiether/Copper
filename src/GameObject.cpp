#include "GameObject.h"

#include <algorithm>
#include <memory>

#include "Component.h"
#include "TileMap.h"

#define MODULE "GameObject"

GameObject::GameObject() : isDead(false) {}
GameObject::~GameObject() {
    components
        .clear();  // pretty sure it would be cleared without this line anyway
}

void GameObject::Update(float dt) {
    for (auto& component : components) {
        component->Update(dt);
    }
}

void GameObject::Render(Vec2 camera) {
    for (auto& component : components) {
        component->Render(camera);
    }
}

bool GameObject::IsDead() { return isDead; }

void GameObject::RequestDelete() { isDead = true; }

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
