#include "state/StageState.h"

#include <algorithm>
#include <memory>

#include "Consts.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Prefabs.h"
#include "component/InfiniteBg.h"
#include "component/IsoCollider.h"
#include "component/Sound.h"
#include "math/Rect.h"
#include "physics/CollisionEngine.h"
#include "util.h"

#define MODULE "StageState"

StageState::~StageState() {
    delete music;
    objects.clear();
}

void StageState::Start() {
    LoadAssets();

    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/sala_v0.png"});
    bgGO->renderLayer = -1;
    RequestAddObject(bgGO);

    for (auto go : MakeMap1Colliders()) {
        RequestAddObject(go);
    }

    auto player = MakePlayer();
    camera->Follow(player);
    RequestAddObject(player);

    // Enemies
    RequestAddObject(MakeEnemyFollower()->WithFootAt({1700, 400}));
    RequestAddObject(MakeEnemyDistancer()->WithFootAt({1700, 400}));

    RequestAddObject(MakeBarril()->WithFootAt({1300, 600}));
    RequestAddObject(MakeEscavadeira()->WithFootAt({1200, 700}));

    // Escavadeira 2
    {
        auto go = MakeEscavadeira()->WithFootAt({1200, 700});
        auto collider = (IsoCollider*)go->GetComponent(CType::IsoCollider);
        Vec2<Cart> center = go->box.Center();
        go->box.SetCenter(
            (center.toIso() + Vec2<Iso>{collider->base.w, 0}).toCart());
        RequestAddObject(go);
    }

    StartArray();
    started = true;

    // music->Play();
}

void StageState::Pause() {}

void StageState::Resume() {}

void StageState::LoadAssets() {
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

    if (input.KeyPress(SDL_SCANCODE_C)) {
        Consts::GetInt("debug.show_colliders") ^= 1;
    }

    // Handle updates
    CollisionEngine::Update(objects);
    for (auto& go : objects) {
        go->Update(dt);
    }
    CollisionEngine::Solve();
    camera->Update(dt);

    // Swap-remove dead objects
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
