#pragma once

#include <string>

#include "Component.h"
#include "SDL_include.h"
using std::string;

class Sprite : Component {
   private:
    SDL_Texture* texture;
    int width, height;
    SDL_Rect clipRect;

   public:
    Sprite(GameObject& associated);
    Sprite(GameObject& associated, const string& file);
    ~Sprite();

    void Open(const string& file);
    void SetClip(int x, int y, int w, int h);

    int GetWidth();
    int GetHeight();
    bool IsOpen();

    void Update(float dt);
    void Render();
    bool Is(const string& type);
};
