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
#include "component/Tilemap.h"
#include "component/Tileset.h"
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

    //////////////////////////////
    //        Background        //
    //////////////////////////////
    // auto bgGO = new GameObject;
    // bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    // bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/sala_v0.png"});
    // bgGO->renderLayer = -2;
    // RequestAddObject(bgGO);

    // for (auto go : MakeMap1Colliders()) {
    //     RequestAddObject(go);
    // }

    //////////////////////////
    //        Player        //
    //////////////////////////
    auto player = MakePlayer()->WithFootAt({0, 0});
    camera->SetPos(player->box.Center());
    camera->Follow(player);
    RequestAddObject(player);

    ///////////////////////////
    //        Enemies        //
    ///////////////////////////
    RequestAddObject(MakeEnemyFollower()->WithFootAt({1700, 400}));
    RequestAddObject(MakeEnemyDistancer()->WithFootAt({1700, 400}));

    ///////////////////////////
    //        Terrain        //
    ///////////////////////////
    auto isoOff = [&](GameObject* go, int xaxis = 1, int yaxis = 0) {
        auto collider = (IsoCollider*)go->GetComponent(CType::IsoCollider);
        auto center = go->box.Center().toIso();
        auto offset =
            Vec2<Iso>{collider->base.w * xaxis, collider->base.h * yaxis};
        go->box.SetCenter((center + offset).toCart());
        return go;
    };

    RequestAddObject(MakeBarril()->WithFootAt({-100, -300}));
    RequestAddObject(MakeEscavadeira()->WithFootAt({-300, 200}));
    RequestAddObject(isoOff(MakeEscavadeira()->WithFootAt({-300, 200})));

    RequestAddObject(MakeVigaB()->WithFootAt({400, 100}));
    RequestAddObject(isoOff(MakeVigaB()->WithFootAt({400, 100}), 0, 1));
    RequestAddObject(isoOff(MakeVigaB()->WithFootAt({400, 100}), 0, 2));
    RequestAddObject(isoOff(MakeVigaB()->WithFootAt({400, 100}), 0, 3));

    auto addTilemap = [&](const char* tileset, int tscols, int tsrows,
                          const char* csv, Vec2<Cart> offset,
                          bool mainMap = false) {
        auto go = new GameObject{};
        go->AddComponent((new Tileset{*go, tileset, tscols, tsrows})
                             ->WithScale(128.0f / 256.0f));
        auto tilemap = (new Tilemap{*go, csv})->WithOffset(offset);
        go->AddComponent(tilemap);
        go->renderLayer = -1;
        go->box.SetTopLeft((Vec2<Iso>{154, 122} * -128).toCart());
        auto wgo = RequestAddObject(go);
        if (mainMap) {
            this->mainMap = wgo;
            tilemap->ComputeComponents();
            for (auto& c : tilemap->Components) {
                info2("component [%d, %d]", c.i, c.j);
            }
        }
    };
    addTilemap(ASSETS "/map/Padrão.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_FLOOR.csv", {0, 0},
               true);
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 11.csv",
               {65, -2});
    // addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
    //            ASSETS "/map/Salas copper V2_Copy of Group 2_Tile
    //            Layer 13.csv");
    // addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
    //            ASSETS "/map/Salas copper V2_Copy of Group 2_Tile
    //            Layer 12.csv");
    // addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
    //            ASSETS "/map/Salas copper V2_Copy of Group 2_Tile
    //            Layer 12.csv");

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
