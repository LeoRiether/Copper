#include "component/LifeBarManager.h"

#include "component/Player.h"
#include "Game.h"

#define MODULE "LifeBarManager"

LifeBarManager::LifeBarManager(GameObject &go, int maxHP, Bar *lifeBar)
    : Component(go) {
    this->maxHP = maxHP;
    this->lifeBar = lifeBar;
    prevHP = 0;
    prevLoss = 0;
}

void LifeBarManager::SetLifeBar(Bar *bar) { lifeBar = bar; }

void LifeBarManager::Update(float) {
    int hp = Player::player->hp;
    if (hp != prevHP) {
        prevHP = hp;
        UpdateHP(hp);
    }
    // int hpLoss = Player::player->hp;
    // if (hpLoss != prevLoss) {
    //     prevLoss = hpLoss;
    // }
    remTime = Player::player->hpLossTimer.Get();
}

void LifeBarManager::UpdateHP(int hp) {
    float hpUnit = maxHP / lifeBar->maxBar;
    lifeBar->SetBarState((int)hp / hpUnit);
}

void LifeBarManager::Render(Vec2<Cart> camera) {
    if(remTime > 0) {
        constexpr float width = 64;
        constexpr float height = 8;
        Vec2<Cart> pos {170, 100};
        const float timer = width * (1 - remTime);

        auto renderer = Game::Instance().Renderer();
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_FRect bgrect{pos.x - width / 2.0f - 2, pos.y - height - 2, width + 4,
                        height + 4};
        // R, G, B, alpha
        SDL_SetRenderDrawColor(renderer, 221, 166, 80, 255);
        SDL_RenderFillRectF(renderer, &bgrect);

        SDL_FRect barrect{pos.x - width / 2.0f, pos.y - height, timer, height};
        SDL_SetRenderDrawColor(renderer, 218, 119, 86, 255);
        SDL_RenderFillRectF(renderer, &barrect);
    }
}
