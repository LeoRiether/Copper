#include "GameObject.h"

#include <algorithm>
#include <memory>

#include "Component.h"
#include "Game.h"
#include "component/Collider.h"

#define MODULE "GameObject"

GameObject::GameObject() {}
GameObject::~GameObject() {
    components
        .clear();  // pretty sure it would be cleared without this line anyway
}

void GameObject::Start() {
    if (started) return;
    for (auto& [key, cs] : components) {
        for (auto& component : cs) component->Start();
    }
}

void GameObject::Update(float dt) {
    Collider* collider = nullptr;
    for (auto& [key, cs] : components) {
        for (auto& component : cs) component->Update(dt);
    }
    if (collider) collider->Update(dt);
}

void GameObject::Render(Vec2<Cart> camera) {
    for (auto& [key, cs] : components) {
        for (auto& component : cs) component->Render(camera);
    }
}

bool GameObject::IsDead() const { return isDead; }

void GameObject::RequestDelete() { isDead = true; }
void GameObject::RequestAdd(GameObject* go) {
    Game::Instance().GetState().RequestAddObject(go);
}

GameObject* GameObject::AddComponent(Component* cmp) {
    components[cmp->Key()].emplace_back(cmp);
    return this;
}

// NOTE: components are removed without maintaining order
GameObject* GameObject::RemoveComponent(Component* cmp) {
    auto key = cmp->Key();
    auto& cs = components[key];
    for (size_t i = 0; i < cs.size(); i++) {
        if (cs[i].get() == cmp) {
            std::swap(cs[i], cs.back());
            cs.pop_back();
            return this;
        }
    }
    return this;
}

// Should return an std::optional<Component*>, really
Component* GameObject::GetComponent(CType key) const {
    auto cs = components.find(key);
    if (cs == components.end() || cs->second.size() == 0) return nullptr;
    return cs->second[0].get();
}

vector<unique_ptr<Component>>& GameObject::GetAllComponents(CType key) {
    return components[key];
}

GameObject* GameObject::WithFootAt(Vec2<Cart> position) {
    this->box.SetFoot(position);
    return this;
}

GameObject* GameObject::WithCenterAt(Vec2<Cart> position) {
    this->box.SetCenter(position);
    return this;
}

void GameObject::NotifyCollision(GameObject& other) {
    for (auto& [key, cs] : components) {
        for (auto& component : cs) component->NotifyCollision(other);
    }
}

void GameObject::NotifyCollisionEnter(GameObject& other) {
    for (auto& [key, cs] : components) {
        for (auto& component : cs) component->NotifyCollisionEnter(other);
    }
}

void GameObject::NotifyCollisionLeave(GameObject& other) {
    for (auto& [key, cs] : components) {
        for (auto& component : cs) component->NotifyCollisionLeave(other);
    }
}
