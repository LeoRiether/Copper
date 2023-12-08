#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "Music.h"
#include "SDL_include.h"
#include "component/Sprite.h"
#include "component/Tilemap.h"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

class State {
   protected:
    bool quitRequested{false};
    bool started{false};

    unique_ptr<Camera> camera{new Camera()};
    vector<shared_ptr<GameObject>> objects{};
    vector<shared_ptr<GameObject>> addRequests{};

    virtual void ProcessAddRequests();
    virtual void StartArray();
    virtual void UpdateArray(float dt);
    virtual void RenderArray();

    struct SortableObject {
        enum { Object, Tile } tag;
        GameObject* go;
        Tilemap::TileToRender tile;
    };

    /* Sorts objects by some notion of depth */
    vector<SortableObject> ZSort();

   public:
    State();
    virtual ~State();
    virtual void LoadAssets() = 0;
    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    virtual weak_ptr<GameObject> RequestAddObject(GameObject* obj);
    virtual weak_ptr<GameObject> GetObject(GameObject* obj);

    inline bool QuitRequested() const { return quitRequested; }
    inline Camera& GetCamera() const { return *camera.get(); }
	bool playerMovement;
};
