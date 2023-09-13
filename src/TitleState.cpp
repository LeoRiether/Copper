#include "TitleState.h"

#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SDL_scancode.h"
#include "StageState.h"
#include "util.h"

#define MODULE "TitleState"

TitleState::TitleState() {}

TitleState::~TitleState() {}

void TitleState::LoadAssets() {}

void TitleState::Update(float dt) {
    ProcessAddRequests();

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        quitRequested = true;
    }

    if (input.KeyPress(SDL_SCANCODE_SPACE)) {
        log("pushing StageState");
        Game::Instance().Push(new StageState{});
    }

    for (auto& go : objects) go->Update(dt);
}

void TitleState::Render() { RenderArray(); }

void TitleState::Start() {
    auto go = new GameObject{};
    go->AddComponent(new Sprite{*go, ASSETS "/img/title.jpg"});
    RequestAddObject(go);
}

void TitleState::Pause() {}

void TitleState::Resume() {}
