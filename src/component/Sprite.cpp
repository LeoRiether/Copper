#include "component/Sprite.h"

#include <chrono>

#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "SDL_render.h"
#include "math/Vec2.h"

#define MODULE "Sprite"

Sprite::Sprite(GameObject& associated, const string& file)
    : Component(associated) {
    Open(file);
}

Sprite::~Sprite() {}

void Sprite::Open(const string& file) {
    texture = Resources::Image(file);

    int qt_status = SDL_QueryTexture(texture->inner, nullptr, nullptr,
                                     &sheetWidth, &sheetHeight);
    if (qt_status != 0)
        fail2("couldn't query texture " YELLOW "'%s'" RESET "! Reason: %s",
              file.c_str(), SDL_GetError());

    SetClip(0, 0, sheetWidth, sheetHeight);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect = SDL_Rect{x, y, w, h};
}
void Sprite::SetClip(SDL_Rect rect) { clipRect = rect; }

void Sprite::Update(float) {}

void Sprite::RenderAt(int x, int y) {
    Game& game = Game::Instance();
    SDL_Rect destRect{x, y, int(clipRect.w * scale.x),
                      int(clipRect.h * scale.y)};
    SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect, &destRect,
                     associated.angle * 180 / PI, nullptr, SDL_FLIP_NONE);
}

void Sprite::Render(Vec2<Cart> camera) {
    RenderAt((int)(associated.box.x - camera.x),
             (int)(associated.box.y - camera.y));
}

bool Sprite::Is(CType type) { return type == CType::Sprite; }
