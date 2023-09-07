#include "Sprite.h"

#include "Game.h"
#include "GameObject.h"
#include "Resources.h"
#include "Vec2.h"

#define MODULE "Sprite"

Sprite::Sprite(GameObject& associated)
    : Component(associated), texture(nullptr) {}

Sprite::Sprite(GameObject& associated, const string& file)
    : Sprite(associated) {
    info2("loading sprite %s", file.c_str());
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

int Sprite::Width() { return width; }

int Sprite::Height() { return height; }

bool Sprite::IsOpen() { return texture != nullptr; }

void Sprite::Update(float) {}

void Sprite::Render(int x, int y) {
    Game& game = Game::Instance();
    SDL_Rect destRect{x, y, clipRect.w, clipRect.h};
    SDL_RenderCopy(game.Renderer(), texture, &clipRect, &destRect);
}

void Sprite::Render(Vec2 camera) {
    Render((int)(associated.box.x - camera.x),
           (int)(associated.box.y - camera.y));
}

bool Sprite::Is(CType type) { return type == CType::Sprite; }
