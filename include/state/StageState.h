#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "Music.h"
#include "SDL_include.h"
#include "State.h"
#include "component/Sprite.h"
#include "math/Vec2.h"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class StageState : public State {
   private:
    Music* music;

    GameObject* CreatePlayer();

   public:
    ~StageState();

    void Start();
    void Pause();
    void Resume();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Input();

    inline Camera& GetCamera() { return *camera; }
};
