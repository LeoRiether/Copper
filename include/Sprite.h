#pragma once

#include <string>

#include "SDL_include.h"
using std::string;


class Sprite {
   private:
    SDL_Texture* texture;
    int width, height;
    SDL_Rect clipRect;

   public:
    Sprite();
    Sprite(const string& file);
    ~Sprite();

    void Open(const string& file);
    void SetClip(int x, int y, int w, int h);
    void Render(int x, int y);

    int GetWidth();
    int GetHeight();
    bool IsOpen();
};
