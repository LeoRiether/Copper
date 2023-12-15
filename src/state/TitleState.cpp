#include "state/TitleState.h"

#include "Colors.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Music.h"
#include "component/Animation.h"
#include "component/Text.h"
#include "component/TextBlinker.h"
#include "component/TextFadeIn.h"
#include "Prefabs.h"
#include "state/StageState.h"
#include "state/ViewerState.h"
#include "util.h"

#define MODULE "TitleState"

TitleState::TitleState() {}

TitleState::~TitleState() {}

void TitleState::LoadAssets() {
    mechanismGlitch =
        unique_ptr<Music>(new Music(
            ASSETS "/audio/Gregor Quendel - Designed Series/Mecha/Sequences - "
                   "Mechanisms 01.wav"));
}

void TitleState::Update(float dt) {
    ProcessAddRequests();

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        quitRequested = true;
    }

    if (input.KeyPress(SDL_SCANCODE_SPACE) ||
        input.ControllerPress(SDL_CONTROLLER_BUTTON_A)) {
        log("pushing StageState");
		MakeOneOffAudio(ASSETS "/audio/metal-pipe.wav");
        Game::Instance().RequestPush(new StageState{});
    }

    if (input.KeyPress(SDL_SCANCODE_V)) {
        Game::Instance().RequestPush(new ViewerState{});
        return;
    }

    for (auto& go : objects) go->Update(dt);
}

void TitleState::Render() { RenderArray(); }

void TitleState::Start() {
    LoadAssets();

    {
        auto bgGO = new GameObject{};
        auto sprite = new Sprite{*bgGO, ASSETS "/img/Capa.png"};
        sprite->SetScale((float)SCREEN_WIDTH / sprite->SheetWidth());
        bgGO->AddComponent(sprite);
        RequestAddObject(bgGO);
    }

    // {
    //     auto go = new GameObject{};
    //     go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 120,
    //                               Text::Blended, "COPPER",
    //                               colorFromHex("CB6D51")});
    //     go->AddComponent(new TextFadeIn{*go, 4.0f});
    //     // go->box.SetFoot(Vec2<Cart>{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 64});
    //     go->box.SetCenter({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f});
    //     RequestAddObject(go);
    // }

    {
        auto go = new GameObject{};
        auto& input = InputManager::Instance();
        auto text =
            input.HasController() ? "press X to start" : "press SPACE to start";
        go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 30,
                                  Text::Blended, text, colorFromHex("dda08d")});
        go->AddComponent(new TextBlinker{*go, 4.0f});
        go->box.SetCenter({SCREEN_WIDTH / 2.0f + 200, SCREEN_HEIGHT / 2.0f + 100});
        RequestAddObject(go);
    }

    mechanismGlitch->Play();
}

void TitleState::Pause() { mechanismGlitch->Stop(); }

void TitleState::Resume() { mechanismGlitch->Play(); }
