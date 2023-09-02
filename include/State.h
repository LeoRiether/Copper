#pragma once

#include "SDL_include.h"
#include "Music.h"
#include "Sprite.h"

#define MODULE "State"

class State {
   private:
    Sprite bg;
    Music music;
    bool quitRequested;

   public:
    State();

    bool QuitRequested();

    void LoadAssets();
    void Update(float dt);
    void Render();
};
