#include "State.h"

#include "util.h"

State::State() : quitRequested(false) {
    info("initializing");
    LoadAssets();
    music->Play();
    info("initialized");
}

State::~State() {
    delete bg;
    delete music;
}

bool State::QuitRequested() { return quitRequested; }

void State::LoadAssets() {
    bg = new Sprite(ASSETS "/img/ocean.jpg");
    music = new Music(ASSETS "/audio/stageState.ogg");
}

void State::Update(float dt) { quitRequested |= SDL_QuitRequested(); }

void State::Render() { bg->Render(0, 0);}
