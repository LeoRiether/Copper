#pragma once

#include "SDL_include.h"

// Wraps a TTF_Font and provides RAII semantics
struct Font {
    TTF_Font* inner;
    ~Font() { TTF_CloseFont(inner); }
};
