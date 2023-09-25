#pragma once

#include "SDL_include.h"

// Wraps a TTF_Font and provides RAII semantics
struct MyFont {
    TTF_Font* inner;
    ~MyFont() { TTF_CloseFont(inner); }
};
