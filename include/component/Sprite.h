#pragma once

#include <string>

#include "CType.h"
#include "Component.h"
#include "SDL_include.h"
#include "Timer.h"
#include "wrap/Texture.h"
using std::shared_ptr;
using std::string;

class Sprite : public Component {
   private:
    shared_ptr<Texture> texture;
    int sheetWidth, sheetHeight;
    Vec2<Cart> scale{1, 1};
    SDL_Rect clipRect;

   public:
    const char* DebugName() { return "Sprite"; }

    Sprite(GameObject& associated, const string& file);
    ~Sprite();

    void Open(const string& file);
    void SetClip(int x, int y, int w, int h);
    void SetClip(SDL_Rect rect);

    inline int SheetWidth() { return sheetWidth; }
    inline int SheetHeight() { return sheetHeight; }
    inline Vec2<Cart> Scale() { return scale; }
    inline void SetScale(Vec2<Cart> s) { scale = s; }
    inline void SetScale(float x) { scale = {x, x}; }

    inline bool IsOpen() { return texture != nullptr; }

    void Update(float dt);
    void RenderAt(int x, int y);
    void Render(Vec2<Cart> camera);
    bool Is(CType type);
};
