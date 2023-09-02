#include "Sprite.h"

#include "Game.h"

#define MODULE "Sprite"

Sprite::Sprite() : texture(nullptr) {}

Sprite::Sprite(const string& file) {
    info2("loading sprite %s", file.c_str());
    Open(file);
}

Sprite::~Sprite() {
    if (texture) SDL_DestroyTexture(texture);
}

void Sprite::Open(const string& file) {
    texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
    if (!texture)
        fail2("couldn't open texture " YELLOW "'%s'" RESET "!", file.c_str());

    int qt_status =
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    if (qt_status)
        fail2("couldn't query texture " YELLOW "'%s'" RESET "! Reason: %s",
              file.c_str(), SDL_GetError());

    SetClip(0, 0, width, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect = SDL_Rect{x, y, w, h};
}

void Sprite::Render(int x, int y) {
    Game& game = Game::GetInstance();
    auto destRect = SDL_Rect{x, y, clipRect.w, clipRect.h};
    SDL_RenderCopy(game.GetRenderer(), texture, &clipRect, &destRect);
}

int Sprite::GetWidth() { return width; }

int Sprite::GetHeight() { return height; }

bool Sprite::IsOpen() { return texture != nullptr; }
