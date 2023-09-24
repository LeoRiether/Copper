#include "component/Sprite.h"

#include <chrono>

#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "SDL_render.h"
#include "math/Vec2.h"

#define MODULE "Sprite"

Sprite::Sprite(GameObject& associated)
    : Component(associated), texture(nullptr) {}

Sprite::Sprite(GameObject& associated, const string& file, int frameCount,
               float frameTime, float secondsToSelfDestruct)
    : Component(associated),
      texture(nullptr),
      frameCount(frameCount),
      frameTime(frameTime),
      secondsToSelfDestruct(secondsToSelfDestruct) {
    Open(file);
}

Sprite::~Sprite() {}

void Sprite::Open(const string& file) {
    texture = Resources::Image(file);

    int qt_status =
        SDL_QueryTexture(texture->inner, nullptr, nullptr, &width, &height);
    if (qt_status != 0)
        fail2("couldn't query texture " YELLOW "'%s'" RESET "! Reason: %s",
              file.c_str(), SDL_GetError());

    SetClip(0, 0, width / frameCount, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect = SDL_Rect{x, y, w, h};
    associated.box.w = w;
    associated.box.h = h;
}

void Sprite::Update(float) {
    while (frameTimeElapsed.Get() >= frameTime) {
        frameTimeElapsed.Restart();
        SetFrame(currentFrame + 1 >= frameCount
                     ? (currentFrame + 1 - frameCount)
                     : (currentFrame + 1));
    }

    if (secondsToSelfDestruct > 0) {
        if (selfDestructCount.Get() >= secondsToSelfDestruct)
            associated.RequestDelete();
    }
}

void Sprite::Render(int x, int y) {
    // TODO: render sprites centered at (x, y)
    Game& game = Game::Instance();
    SDL_Rect destRect{x, y, int(clipRect.w * scale.x),
                      int(clipRect.h * scale.y)};
    SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect, &destRect,
                     associated.angle * 180 / PI, nullptr, SDL_FLIP_NONE);
}

void Sprite::Render(Vec2<Cart> camera) {
    Render((int)(associated.box.x - camera.x),
           (int)(associated.box.y - camera.y));
}

bool Sprite::Is(CType type) { return type == CType::Sprite; }

void Sprite::SetScale(float scaleX, float scaleY) {
    SetScale(Vec2<Cart>{scaleX, scaleY});
}
void Sprite::SetScale(Vec2<Cart> s) { scale = s; }

void Sprite::SetFrame(int frame) {
    currentFrame = frame;
    clipRect.x = 1ll * frame * width / frameCount;
}
void Sprite::SetFrameCount(int fc) {
    frameCount = fc;
    SetClip(0, 0, width / frameCount, height);
}
void Sprite::SetFrameTime(float ft) { frameTime = ft; }
