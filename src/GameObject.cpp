#include "GameObject.h"

#include <algorithm>

#include "Component.h"

#define MODULE "GameObject"

GameObject::GameObject() : components({}), isDead(false) {}
GameObject::~GameObject() {
    for (auto& component : components) {
        delete component;
    }
    components
        .clear();  // pretty sure it would be cleared without this line anyway
}

void GameObject::Update(float dt) {
    for (auto& component : components) {
        component->Update(dt);
    }
}

void GameObject::Render() {
    for (auto& component : components) {
        component->Render();
    }
}

bool GameObject::IsDead() { return isDead; }

void GameObject::RequestDelete() { isDead = true; }

void GameObject::AddComponent(Component* cmp) { components.push_back(cmp); }
void GameObject::RemoveComponent(Component* cmp) {
    components.erase(std::find(components.begin(), components.end(), cmp));
}

// Should return an std::optional<Component*>, really
Component* GameObject::GetComponent(const string& type) {
    for (const auto& component : components) {
        if (component->Is(type)) return component;
    }
    return nullptr;
}
