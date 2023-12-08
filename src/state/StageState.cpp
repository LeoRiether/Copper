#include "state/StageState.h"

#include <algorithm>
#include <memory>

#include "Consts.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Prefabs.h"
#include "StagePrefabs.h"
#include "component/FPSCounter.h"
#include "component/InfiniteBg.h"
#include "component/Sound.h"
#include "component/Dialog.h"
#include "component/Tilemap.h"
#include "component/Tileset.h"
#include "physics/CollisionEngine.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "StageState"

StageState::~StageState() {
    delete music;
    objects.clear();
}

void StageState::Start() {
    LoadAssets();

    ///////////////////////////////
    //        FPS Counter        //
    ///////////////////////////////
    auto fpsGo = new GameObject;
    fpsGo->AddComponent(new FPSCounter{*fpsGo});
    RequestAddObject(fpsGo);

    //////////////////////////////
    //        Load Stage        //
    //////////////////////////////
    MakeStage1(*this, 1);

    /////////////////////////////////////
    //        Controls Tutorial        //
    /////////////////////////////////////
    if (GameData::firstStage) {
        GameData::firstStage = false;
        RequestAddObject(MakeControlsTutorial());
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

    if (input.KeyPress(SDL_SCANCODE_P)) {
        warn2("mouse pos: { %d, %d }", input.MouseX(), input.MouseY());
        auto mi = input.Mouse().toIso();
        warn2("iso mouse pos: { %g, %g }", mi.x, mi.y);
    }

    if (input.KeyPress(SDL_SCANCODE_0)) {
        for (auto& go : objects) {
            if (go->tags.test(tag::Enemy))
                go->RequestDelete();
        }
    }

	if (input.KeyPress(INTERACT_KEY))
		RequestAddObject(MakeDialog(ASSETS "/dialog/sample.txt"));

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

            if (objects[i]->tags.test(tag::Enemy)) EnemyCount--;

            std::swap(objects[i], objects.back());
            objects.pop_back();
        } else {
            i++;
        }
    }
}

void StageState::Render() { RenderArray(); }
