#pragma once

#include "SDL_include.h"
#include <string> 
using std::string;

class Music {
   private:
    Mix_Music* music;

   public:
    Music();
    Music(const string& file);
    ~Music();

    void Play(int times = -1);
    void Stop(int msToStop = 1500);
    void Open(const string& file);
    bool IsOpen();
};
