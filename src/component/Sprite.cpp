#include "component/Sprite.h"

#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "component/Collider.h"
#include "math/Vec2.h"
#include "util.h"

#define MODULE "Sprite"

Sprite::Sprite(GameObject& associated, const string& file, bool followsCamera)
    : Component(associated){
    Open(file);

    this->followsCamera = followsCamera;
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

Sprite* Sprite::WithFlash(float timeout) {
    flashTimeout = timeout;
    return this;
}

void Sprite::Update(float dt) { flashTimeout -= dt; }

void Sprite::RenderAt(float x, float y) {
    Game& game = Game::Instance();
    SDL_FRect destRect{x, y, clipRect.w * scale.x, clipRect.h * scale.y};

    if (hasShadow) {
        auto col = (Collider*)associated.GetComponent(CType::Collider);
        float shadowX = x;
        float shadowY = y + (col ? col->base.y * scale.y + col->base.h - 10
                                 : clipRect.h * scale.y);

        SDL_FRect destRect{shadowX, shadowY, clipRect.w * scale.x,
                           clipRect.h * scale.y * 0.5f};
        SDL_FPoint rotationPoint{destRect.w / 2.0f, 0};
        SDL_SetTextureColorMod(texture->inner, 0, 0, 0);
        SDL_SetTextureAlphaMod(texture->inner, 128);
        SDL_RenderCopyExF(game.Renderer(), texture->inner, &clipRect, &destRect,
                          0, &rotationPoint, SDL_FLIP_VERTICAL);
        SDL_SetTextureAlphaMod(texture->inner, 255);
        SDL_SetTextureColorMod(texture->inner, 255, 255, 255);
    }

    SDL_SetTextureAlphaMod(texture->inner, Alpha);
    SDL_RenderCopyExF(game.Renderer(), texture->inner, &clipRect, &destRect,
                      associated.angle * 180 / PI, nullptr, fipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    if (flashTimeout > 0) {
        SDL_SetTextureBlendMode(texture->inner, SDL_BLENDMODE_ADD);
        for (int i = 0; i < 3; i++)
            SDL_RenderCopyExF(game.Renderer(), texture->inner, &clipRect,
                              &destRect, associated.angle * 180 / PI, nullptr,
                              SDL_FLIP_NONE);
        SDL_SetTextureBlendMode(texture->inner, SDL_BLENDMODE_BLEND);
    }
}

void Sprite::Render(Vec2<Cart> camera) {
    if (followsCamera) {
        RenderAt(associated.box.x, associated.box.y);
    } else {
        RenderAt(associated.box.x - camera.x, associated.box.y - camera.y);
    }
}
