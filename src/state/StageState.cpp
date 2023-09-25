#include "state/StageState.h"

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "CType.h"
#include "Collider.h"
#include "Collision.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SDL_scancode.h"
#include "Sound.h"
#include "TileSet.h"
#include "component/Alien.h"
#include "component/CameraFollower.h"
#include "component/InfiniteBg.h"
#include "component/KeepSoundAlive.h"
#include "component/Player.h"
#include "component/TileMap.h"
#include "math/Rect.h"
#include "state/TitleState.h"
#include "util.h"

#define MODULE "StageState"

GameObject* StageState::CreatePlayer() {
    auto go = new GameObject{};
    auto body = new Player{*go};
    go->AddComponent(body);
    go->box.SetCenter(Vec2<Cart>{704, 640});
    return go;
}

StageState::~StageState() {
    delete music;
    objects.clear();
}

void StageState::Start() {
    LoadAssets();

    // Player
    auto player = CreatePlayer();
    camera->Follow(player);
    RequestAddObject(player);

    StartArray();
    started = true;

    // music->Play();
}

void StageState::Pause() {}

void StageState::Resume() {}

void StageState::LoadAssets() {
    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/isobg.png"});
    RequestAddObject(bgGO);

    // Background music
    music = new Music(ASSETS "/audio/stageState.ogg");
}

void StageState::Update(float dt) {
    // process add requests here so added objects are updated before their first
    // render
    ProcessAddRequests();

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested()) {
        quitRequested = true;
    }

    if (input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        Game::Instance().RequestPop();
        return;
    }

    // Handle updates
    vector<GameObject*> collidableObjects;
    for (auto& go : objects) {
        go->Update(dt);

        if (go->GetComponent(CType::Collider))
            collidableObjects.push_back(go.get());
    }

    camera->Update(dt);

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

void StageState::Render() { RenderArray(); }
