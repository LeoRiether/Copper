#pragma once

#include "Music.h"
#include "State.h"

using std::unique_ptr;

class TitleState : public State {
   public:
    TitleState();
    ~TitleState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

   private:
    unique_ptr<Music> mechanismGlitch;
};
