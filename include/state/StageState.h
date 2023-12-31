#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Music.h"
#include "SDL_include.h"
#include "State.h"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

class StageState : public State {
   private:
    Music* music;

   public:
    weak_ptr<GameObject> mainMap;

    /** Counts number of GameObjects where go->tag.test(tag::Enemy) is true.
     * It's automatically updated when RequestAdds and RequestDeletes are
     * processed  */
    int EnemyCount{0};

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
