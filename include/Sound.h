#pragma once

#include <string>

#include "Component.h"
#include "GameObject.h"
#include "SDL_include.h"
#include "util.h"
using std::string;

class Sound : Component {
   private:
    Mix_Chunk* chunk;
    int channel;

   public:
    Sound(GameObject& associated);
    Sound(GameObject& associated, const string& file);
    ~Sound();

    void Play(int times = 1);
    void Stop();
    void Open(const string& file);
    bool IsOpen();

    void Update(float dt);
    void Render();
    bool Is(const string& type);
};
