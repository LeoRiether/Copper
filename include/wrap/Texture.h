#pragma once

#include "SDL_include.h"

// Wraps an SDL_Texture and provides RAII semantics
struct Texture {
    SDL_Texture* inner;
    inline ~Texture() { SDL_DestroyTexture(inner); }
};
