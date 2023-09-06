#pragma once

#include <string>

#include "CType.h"
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

    int Width();
    int Height();
    bool IsOpen();

    void Update(float dt);
    void Render(int x, int y);
    void Render(Vec2 camera);
    bool Is(CType type);
};
