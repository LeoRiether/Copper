#pragma once

#include <memory>
#include <shared_mutex>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "Music.h"
#include "SDL_include.h"
#include "Sprite.h"
#include "Vec2.h"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

class State {
   private:
    Music* music;
    Camera* camera;
    bool quitRequested;
    bool started;
    vector<shared_ptr<GameObject>> objects;
    vector<shared_ptr<GameObject>> addRequests;

    void ProcessAddRequests();

   public:
    State();
    ~State();
    void Start();

    bool QuitRequested();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Input();
    weak_ptr<GameObject> RequestAddObject(GameObject* go);
    weak_ptr<GameObject> GetObject(GameObject* go);

    inline Camera& GetCamera() { return *camera; }
};
