#include "Sprite.h"

#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "SDL_render.h"
#include "Vec2.h"

#define MODULE "Sprite"

Sprite::Sprite(GameObject& associated)
    : Component(associated), texture(nullptr) {}

Sprite::Sprite(GameObject& associated, const string& file)
    : Sprite(associated) {
    Open(file);
}

Sprite::~Sprite() {}

void Sprite::Open(const string& file) {
    texture = Resources::Image(file);

    int qt_status =
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    if (qt_status != 0)
        fail2("couldn't query texture " YELLOW "'%s'" RESET "! Reason: %s",
              file.c_str(), SDL_GetError());

    SetClip(0, 0, width, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect = SDL_Rect{x, y, w, h};
    associated.box = Rect{(float)x, (float)y, (float)width, (float)height};
}

void Sprite::Update(float) {}

void Sprite::Render(int x, int y) {
    Game& game = Game::Instance();
    SDL_Rect destRect{x, y, int(clipRect.w * scale.x),
                      int(clipRect.h * scale.y)};
    SDL_RenderCopyEx(game.Renderer(), texture, &clipRect, &destRect,
                     associated.angle * 180 / PI, nullptr, SDL_FLIP_NONE);
}

void Sprite::Render(Vec2 camera) {
    Render((int)(associated.box.x - camera.x),
           (int)(associated.box.y - camera.y));
}

bool Sprite::Is(CType type) { return type == CType::Sprite; }

void Sprite::SetScale(float scaleX, float scaleY) {
    SetScale(Vec2{scaleX, scaleY});
}

void Sprite::SetScale(Vec2 s) { scale = s; }
