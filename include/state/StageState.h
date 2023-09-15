#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "Music.h"
#include "SDL_include.h"
#include "State.h"
#include "Vec2.h"
#include "component/Sprite.h"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

class StageState : public State {
   private:
    Music* music;

    GameObject* CreatePenguin();
    GameObject* CreatePenguinBody(weak_ptr<GameObject> tileMap);
    GameObject* CreateAlien(float x, float y);
    GameObject* CreateAlien();

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
