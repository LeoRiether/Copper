#include "state/State.h"

#include <algorithm>
#include <fstream>
#include <tuple>

#include "util.h"

#define MODULE "State"

State::State() { camera->SetSpeed(Vec2<Cart>{250, 250}); }

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
    ZSort();
    for (auto& go : objects) {
        go->Render(camera->Pos());
    }
}

// Currently (experimentally!!) doing a Shell Sort by <layer, box.y+box.h>
void State::ZSort() {
    auto key = [](const GameObject& go) {
        return std::tuple{go.renderLayer, go.box.y + go.box.h};
    };

    const size_t n = objects.size();
    for (size_t gap : {102, 45, 20, 9, 4, 1}) {
        for (size_t i = gap; i < n; i++) {
            for (size_t j = i;
                 j >= gap && key(*objects[j]) < key(*objects[j - gap]);
                 j -= gap)
                std::swap(objects[j], objects[j - gap]);
        }
    }
}
