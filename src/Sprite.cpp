#include "Sprite.h"

#include "Game.h"
#include "GameObject.h"

#define MODULE "Sprite"

Sprite::Sprite(GameObject& associated)
    : Component(associated), texture(nullptr) {}

Sprite::Sprite(GameObject& associated, const string& file)
    : Sprite(associated) {
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
    if (qt_status != 0)
        fail2("couldn't query texture " YELLOW "'%s'" RESET "! Reason: %s",
              file.c_str(), SDL_GetError());

    SetClip(0, 0, width, height);
}

void Sprite::SetClip(int x, int y, int w, int h) {
    clipRect = SDL_Rect{x, y, w, h};
}

int Sprite::Width() { return width; }

int Sprite::Height() { return height; }

bool Sprite::IsOpen() { return texture != nullptr; }

void Sprite::Update(float dt) {}

void Sprite::Render(int x, int y) {
    Game& game = Game::GetInstance();
    SDL_Rect destRect{x, y, clipRect.w, clipRect.h};
    SDL_RenderCopy(game.GetRenderer(), texture, &clipRect, &destRect);
}

void Sprite::Render() { Render((int)associated.box.x, (int)associated.box.y); }

bool Sprite::Is(CType type) { return type == CType::Sprite; }
