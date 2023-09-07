#pragma once

#include <string>

#include "CType.h"
#include "Component.h"
#include "SDL_include.h"
using std::string;

class Sprite : public Component {
   private:
    SDL_Texture* texture;
    int width, height;
    Vec2 scale{1, 1};
    SDL_Rect clipRect;

   public:
    Sprite(GameObject& associated);
    Sprite(GameObject& associated, const string& file);
    ~Sprite();

    void Open(const string& file);
    void SetClip(int x, int y, int w, int h);

    inline int Width() { return width * scale.x; }
    inline int Height() { return height * scale.y; }
    inline bool IsOpen() { return texture != nullptr; }

    void Update(float dt);
    void Render(int x, int y);
    void Render(Vec2 camera);
    bool Is(CType type);

    inline Vec2 Scale() { return scale; }
    void SetScale(float scaleX, float scaleY);
    void SetScale(Vec2 scale);
};
