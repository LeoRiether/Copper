#pragma once

#include "SDL_include.h"
#include <memory>
#include <string> 
using std::string;
using std::shared_ptr;

class Music {
   private:
    shared_ptr<Mix_Music> music;

   public:
    Music();
    Music(const string& file);
    ~Music();

    void Play(int times = -1);
    void Stop(int msToStop = 1500);
    void Open(const string& file);
    bool IsOpen();
};
