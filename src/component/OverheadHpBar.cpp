#include "component/OverheadHpBar.h"

#include "Game.h"
#include "GameObject.h"
#include "SDL_render.h"

#define MODULE "OverheadHpBar"

OverheadHpBar::OverheadHpBar(GameObject& go, int initialHp, int maxHp)
    : Component(go), hp(initialHp), maxHp(maxHp), shownHp(initialHp) {}

void OverheadHpBar::Update(float) { shownHp += (hp - shownHp) * 0.03; }

void OverheadHpBar::Render(Vec2<Cart> camera) {
    if (hp == maxHp) return;

    constexpr float width = 64;
    constexpr float height = 8;
    const float hpwidth = width * shownHp / maxHp;
    const auto [x, y] = associated.box.Head() - camera;

    auto renderer = Game::Instance().Renderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_FRect bgrect{x - width / 2.0f - 2, y - height - 2, width + 4,
                     height + 4};
    SDL_SetRenderDrawColor(renderer, 92, 34, 13, 255);
    SDL_RenderFillRectF(renderer, &bgrect);

    SDL_FRect barrect{x - width / 2.0f, y - height, hpwidth, height};
    SDL_SetRenderDrawColor(renderer, 255, 210, 45, 255);
    SDL_RenderFillRectF(renderer, &barrect);
}
