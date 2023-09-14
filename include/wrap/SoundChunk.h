#pragma once

#include "SDL_include.h"

// Wraps a Mix_Chunk and provides RAII semantics
struct SoundChunk {
    Mix_Chunk* inner;
    ~SoundChunk() { Mix_FreeChunk(inner); }
};
