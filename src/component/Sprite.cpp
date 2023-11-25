#include "component/Sprite.h"

#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "math/Vec2.h"
#include "util.h"

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

Sprite* Sprite::WithFlash(bool f) {
    flash = f;
    return this;
}

void Sprite::Update(float) {}

void Sprite::RenderAt(int x, int y) {
    Game& game = Game::Instance();
    SDL_Rect destRect{x, y, int(clipRect.w * scale.x),
                      int(clipRect.h * scale.y)};

    if (hasShadow) {
        int shadowX = x;
        int shadowY = y + clipRect.h * scale.y - 20;

        SDL_Rect destRect{shadowX, shadowY, int(clipRect.w * scale.x),
                          int(clipRect.w * scale.x * 0.5)};
        SDL_Point rotationPoint{destRect.w / 2, 0};
        SDL_SetTextureColorMod(texture->inner, 0, 0, 0);
        SDL_SetTextureAlphaMod(texture->inner, 128);
        SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect, &destRect,
                         0, &rotationPoint, SDL_FLIP_VERTICAL);
        SDL_SetTextureAlphaMod(texture->inner, 255);
        SDL_SetTextureColorMod(texture->inner, 255, 255, 255);
    }

    SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect, &destRect,
                     associated.angle * 180 / PI, nullptr, SDL_FLIP_NONE);

    if (flash) {
        SDL_SetTextureBlendMode(texture->inner, SDL_BLENDMODE_ADD);
        for (int i = 0; i < 1; i++)
            SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect,
                             &destRect, associated.angle * 180 / PI, nullptr,
                             SDL_FLIP_NONE);
        SDL_SetTextureBlendMode(texture->inner, SDL_BLENDMODE_BLEND);
    }
}

void Sprite::Render(Vec2<Cart> camera) {
    RenderAt((int)(associated.box.x - camera.x),
             (int)(associated.box.y - camera.y));
}

bool Sprite::Is(CType type) { return type == CType::Sprite; }
