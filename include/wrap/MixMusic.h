#pragma once

#include "SDL_include.h"

// Wraps a Mix_Music and provides RAII semantics
struct MixMusic {
  Mix_Music *inner;
  ~MixMusic() { Mix_FreeMusic(inner); }
};
