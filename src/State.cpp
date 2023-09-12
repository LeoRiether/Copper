#include "State.h"

#include <bits/fs_fwd.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "Alien.h"
#include "CType.h"
#include "CameraFollower.h"
#include "Collider.h"
#include "Collision.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "KeepSoundAlive.h"
#include "PenguinBody.h"
#include "Rect.h"
#include "Sound.h"
#include "TileMap.h"
#include "TileSet.h"
#include "util.h"

#define MODULE "State"

GameObject* CreatePenguin() {
    auto& input = InputManager::Instance();
    Vec2 objPos = Vec2{200, 0}.GetRotated(-PI + PI * (rng() % 1001) / 500.0) +
                  Vec2{(float)input.MouseX(), (float)input.MouseY()};

    auto go = new GameObject{};
    auto sprite = new Sprite(*go, ASSETS "/img/penguinface.png");
    go->box = Rect{objPos.x, objPos.y, (float)sprite->Width(),
                   (float)sprite->Height()};

    auto sound = new Sound(*go, ASSETS "/audio/boom.wav");

    go->AddComponent(sprite);
    go->AddComponent(sound);
    return go;
}

GameObject* CreatePenguinBody() {
    auto go = new GameObject{};
    auto body = new PenguinBody{*go};
    go->AddComponent(body);
    go->box.SetCenter(Vec2{704, 640});
    return go;
}

GameObject* CreateAlien(float x, float y) {
    auto go = new GameObject{};
    auto alien = new Alien{*go, 7};
    auto sprite = (Sprite*)go->GetComponent(CType::Sprite);
    go->box = Rect{x, y, (float)sprite->Width(), (float)sprite->Height()};

    go->AddComponent(alien);
    return go;
}
GameObject* CreateAlien() {
    auto& input = InputManager::Instance();
    Vec2 objPos = Vec2{200, 0}.GetRotated(-PI + PI * (rng() % 1001) / 500.0) +
                  Vec2{(float)input.MouseX(), (float)input.MouseY()};
    return CreateAlien(objPos.x, objPos.y);
}

State::State() : camera(new Camera{}), quitRequested(false), started(false) {
    info("initializing");
    camera->SetSpeed(Vec2{250, 250});
    info("initialized");
}

State::~State() {
    delete music;
    objects.clear();
}

void State::Start() {
    LoadAssets();

    auto penguinBody = CreatePenguinBody();
    camera->Follow(penguinBody);
    RequestAddObject(penguinBody);

    for (auto& go : objects) {
        go->Start();
    }
    ProcessAddRequests();
    started = true;

    music->Play();
}

bool State::QuitRequested() { return quitRequested; }

void State::LoadAssets() {
    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent(new Sprite{*bgGO, ASSETS "/img/ocean.jpg"});
    bgGO->AddComponent(new CameraFollower{*bgGO});
    RequestAddObject(bgGO);

    // Tilemap
    auto tileGO = new GameObject;
    tileGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    auto tileset = new TileSet(DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT,
                               ASSETS "/img/tileset.png");
    auto tilemap = new TileMap(*tileGO, ASSETS "/map/tileMap.txt", tileset);
    tileGO->AddComponent(tilemap);
    RequestAddObject(tileGO);

    // Background music
    music = new Music(ASSETS "/audio/stageState.ogg");
}

void State::Update(float dt) {
    // process add requests here so added objects are updated before their first
    // render
    ProcessAddRequests();

    camera->Update(dt);

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    if (input.KeyPress(SDL_SCANCODE_SPACE)) {
        RequestAddObject(CreateAlien());
    }

    // Handle updates
    vector<GameObject*> collidableObjects;
    for (auto& go : objects) {
        go->Update(dt);

        if (go->GetComponent(CType::Collider))
            collidableObjects.push_back(go.get());
    }

    // Handle collisions
    size_t n = collidableObjects.size();
    for (size_t i = 0; i < n; i++) {
        auto collider_i =
            (Collider*)collidableObjects[i]->GetComponent(CType::Collider);
        for (size_t j = i + 1; j < n; j++) {
            auto collider_j =
                (Collider*)collidableObjects[j]->GetComponent(CType::Collider);
            if (Collision::IsColliding(collider_i->box, collider_j->box,
                                       collidableObjects[i]->angle,
                                       collidableObjects[j]->angle)) {
                collidableObjects[i]->NotifyCollision(*collidableObjects[j]);
                collidableObjects[j]->NotifyCollision(*collidableObjects[i]);
                // log2("%s colliding with %s",
                //      collidableObjects[i]->Components().back()->DebugName(),
                //      collidableObjects[j]->Components().back()->DebugName());
            }
        }
    }

    // swap-remove dead objects
    for (size_t i = 0; i < objects.size();) {
        if (objects[i]->IsDead()) {
            if (camera->Focus() == objects[i].get()) camera->Unfollow();
            std::swap(objects[i], objects.back());
            objects.pop_back();
        } else {
            i++;
        }
    }
}

void State::Render() {
    for (const auto& go : objects) {
        go->Render(camera->Pos());
    }
}

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
