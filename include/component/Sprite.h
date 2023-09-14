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
    int width, height;
    Vec2 scale{1, 1};

    int frameCount, currentFrame{0};
    float timeElapsed{0}, frameTime;

    Timer selfDestructCount;
    float secondsToSelfDestruct{0};

    SDL_Rect clipRect;

   public:
    const char* DebugName() { return "Sprite"; }

    Sprite(GameObject& associated);
    Sprite(GameObject& associated, const string& file, int frameCount = 1,
           float frameTime = 1, float secondsToSelfDestruct = 0);
    ~Sprite();

    void Open(const string& file);
    void SetClip(int x, int y, int w, int h);

    inline int Width() { return width * scale.x / frameCount; }
    inline int Height() { return height * scale.y; }
    inline bool IsOpen() { return texture != nullptr; }

    void Update(float dt);
    void Render(int x, int y);
    void Render(Vec2 camera);
    bool Is(CType type);

    inline Vec2 Scale() { return scale; }
    void SetScale(float scaleX, float scaleY);
    void SetScale(Vec2 scale);

    void SetFrame(int frame);
    void SetFrameCount(int frameCount);
    void SetFrameTime(float frameTime);
};
