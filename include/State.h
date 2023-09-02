#pragma once

#include "Music.h"
#include "SDL_include.h"
#include "Sprite.h"

#define MODULE "State"

class State {
   private:
    Music* music;
    Sprite* bg;
    bool quitRequested;

   public:
    State();
    ~State();

    bool QuitRequested();

    void LoadAssets();
    void Update(float dt);
    void Render();
};
