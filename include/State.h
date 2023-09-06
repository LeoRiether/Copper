#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Face.h"
#include "GameObject.h"
#include "Music.h"
#include "SDL_include.h"
#include "Sprite.h"
#include "Vec2.h"

using std::unique_ptr;
using std::vector;

class State {
   private:
    Music* music;
    Camera* camera;
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

    inline Camera& GetCamera() { return *camera; }
};
