#pragma once

#include "GameObject.h"
#include "Music.h"
#include "SDL_include.h"
#include "Sprite.h"
#include "Vec2.h"
#include "Face.h"
#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;

class State {
   private:
    Music* music;
    bool quitRequested;
    vector<unique_ptr<GameObject>> objects;

   public:
    State();
    ~State();

    bool QuitRequested();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Input();
    void AddObject(int mouseX, int mouseY);
};
