#include "component/InfiniteBg.h"

#include "Game.h"
#include "Resources.h"
#include "util.h"

#define MODULE "InfiniteBg"

InfiniteBg::InfiniteBg(GameObject& associated, const string& file)
    : Component(associated) {
    texture = Resources::Image(file);

    int qt_status =
        SDL_QueryTexture(texture->inner, nullptr, nullptr, &width, &height);
    if (qt_status != 0)
        fail2("couldn't query texture " YELLOW "'%s'" RESET "! Reason: %s",
              file.c_str(), SDL_GetError());
}

void InfiniteBg::Update(float) {}

void InfiniteBg::Render(Vec2<Cart> camera) {
    int offX = int(camera.x) % width;
    int offY = int(camera.y) % height;

    Game& game = Game::Instance();

    // Always draw 9 copies of the background
    SDL_Rect clipRect{0, 0, width, height};
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            SDL_Rect destRect{-offX + width * j, -offY + height * i, width,
                              height};
            SDL_RenderCopyEx(game.Renderer(), texture->inner, &clipRect,
                             &destRect, 0, nullptr, SDL_FLIP_NONE);
        }
    }
}

bool InfiniteBg::Is(CType type) { return type == CType::InfiniteBg; }
