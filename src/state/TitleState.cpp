#include "state/TitleState.h"

#include "Colors.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
#include "component/Text.h"
#include "component/TextBlinker.h"
#include "state/StageState.h"
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
        Game::Instance().RequestPush(new StageState{});
    }

    for (auto& go : objects) go->Update(dt);
}

void TitleState::Render() { RenderArray(); }

void TitleState::Start() {
    {
        auto bgGO = new GameObject{};
        bgGO->AddComponent(new Sprite{*bgGO, ASSETS "/img/title.jpg"});
        RequestAddObject(bgGO);
    }

    {
        auto go = new GameObject{};
        go->AddComponent(new Text{*go, ASSETS "/font/Call me maybe.ttf", 70,
                                  Text::Blended, "Aperte espaco para continuar",
                                  colorFromHex("#F0A029")});
        go->AddComponent(new TextBlinker{*go, 1.0f});
        go->box.SetCenter(Vec2<Cart>{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 100});
        RequestAddObject(go);
    }
}

void TitleState::Pause() {}

void TitleState::Resume() {}
