#include "state/EndState.h"

#include "Colors.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Resources.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "component/TextBlinker.h"
#include "util.h"

EndState::EndState() {}

EndState::~EndState() {}

void EndState::LoadAssets() {
    // uhm, I'm doing everything in EndState::Start()...
}

void EndState::Update(float dt) {
    ProcessAddRequests();

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        quitRequested = true;
        return;
    }

    if (input.KeyPress(SDL_SCANCODE_SPACE)) {
        // Go back to the TitleState, which should be already pushed!
        Game::Instance().RequestPop();
    }

    UpdateArray(dt);
}

void EndState::Render() { RenderArray(); }

void EndState::Start() {
    auto bgGO = new GameObject{};

    if (GameData::playerVictory) {
        bgMusic = Music{ASSETS "/audio/endStateWin.ogg"};
        bgGO->AddComponent(new Sprite{*bgGO, ASSETS "/img/win.jpg"});
    } else {
        bgMusic = Music{ASSETS "/audio/endStateLose.ogg"};
        bgGO->AddComponent(new Sprite{*bgGO, ASSETS "/img/lose.jpg"});
    }

    bgMusic.Play();

    bgGO->box.x = 0;
    bgGO->box.y = 0;
    RequestAddObject(bgGO);

    {
        auto text = new GameObject{};
        text->AddComponent(new Text{*text, ASSETS "/font/Call me maybe.ttf", 50,
                                    Text::Blended, "ESC . Sair",
                                    colorFromHex("#F0A029")});
        text->AddComponent(new TextBlinker{*text, 1.5});
        text->box.SetCenter(Vec2{SCREEN_WIDTH / 2.0f, 0});
        text->box.y = SCREEN_HEIGHT - 120;
        RequestAddObject(text);
    }
    {
        auto text = new GameObject{};
        text->AddComponent(new Text{*text, ASSETS "/font/Call me maybe.ttf", 50,
                                    Text::Blended, "Espaco . Jogar de novo",
                                    colorFromHex("#F0A029")});
        text->AddComponent(new TextBlinker{*text, 1.5});
        text->box.SetCenter(Vec2{SCREEN_WIDTH / 2.0f, 0});
        text->box.y = SCREEN_HEIGHT - 180;
        RequestAddObject(text);
    }
}

void EndState::Pause() { bgMusic.Stop(); }

void EndState::Resume() { bgMusic.Play(); }
