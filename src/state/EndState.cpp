#include "state/EndState.h"

#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Resources.h"
#include "component/Sprite.h"
#include "util.h"

EndState::EndState() {}

EndState::~EndState() {}

void EndState::LoadAssets() {
    // uhm, I'm doing everything in EndState::Start()...
}

void EndState::Update(float) {
    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        quitRequested = true;
        return;
    }

    if (input.KeyPress(SDL_SCANCODE_SPACE)) {
        // TODO: go back to title state
    }
}

void EndState::Render() {}

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
    RequestAddObject(bgGO);
}

void EndState::Pause() { bgMusic.Stop(); }

void EndState::Resume() { bgMusic.Play(); }
