#include "State.h"
#include "util.h"

State::State() : quitRequested(false) {
    log("initializing");
    LoadAssets();
    log("loaded assets");
    music.Play();
    log("initialized");
}

bool State::QuitRequested() { return quitRequested; }

void State::LoadAssets() {
    bg = Sprite(ASSETS "/img/ocean.jpg");
    // music = Music(ASSETS "/audio/stageState.ogg");
    music = Music();
}

void State::Update(float dt) { quitRequested |= SDL_QuitRequested(); }

void State::Render() { bg.Render(0, 0); }
