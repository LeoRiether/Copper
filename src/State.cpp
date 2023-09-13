#include "State.h"

#include "util.h"

#define MODULE "State"

State::State() { camera->SetSpeed(Vec2{250, 250}); }

State::~State() {
    objects.clear();
    addRequests.clear();
}

void State::LoadAssets() {}
void State::Start() { started = true; }
void State::Pause() {}
void State::Resume() {}

weak_ptr<GameObject> State::RequestAddObject(GameObject* go) {
    shared_ptr<GameObject> ptr{go};
    addRequests.emplace_back(ptr);
    return ptr;
}

// WARN: partial function, only call if you're sure the GameObject is in
// `objects`
weak_ptr<GameObject> State::GetObject(GameObject* go) {
    for (auto& candidate : objects) {
        if (go == candidate.get()) return candidate;
    }
    fail("GetObject called with GameObject that's not registered");
}

void State::ProcessAddRequests() {
    while (!addRequests.empty()) {
        auto newObjects = std::move(addRequests);
        addRequests = {};
        for (auto& go : newObjects) {
            objects.emplace_back(go);
        }
        for (auto& go : newObjects) {
            go->Start();
        }
    }
}

void State::StartArray() {
    for (auto& go : objects) {
        go->Start();
    }
    ProcessAddRequests();
}

void State::UpdateArray(float dt) {
    for (auto& go : objects) {
        go->Update(dt);
    }
}

void State::RenderArray() {
    for (auto& go : objects) {
        go->Render(camera->Pos());
    }
}
