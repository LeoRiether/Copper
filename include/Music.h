#pragma once

#include <memory>
#include <string>

#include "SDL_include.h"
#include "wrap/MixMusic.h"
using std::shared_ptr;
using std::string;

class Music {
   private:
    shared_ptr<MixMusic> music;

   public:
    Music();
    Music(const string& file);
    ~Music();

    void Play(int times = -1);
    void Stop(int msToStop = 0);
    void Open(const string& file);
    bool IsOpen();
};
