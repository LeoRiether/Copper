#include "component/EndStateDimmer.h"

#include <cmath>

#include "Component.h"
#include "Game.h"
#include "state/EndState.h"

EndStateDimmer::EndStateDimmer(GameObject& associated, float secondsToDeath)
    : Component(associated), secondsToDeath(secondsToDeath) {}

void EndStateDimmer::Update(float dt) {
    timer += dt;
    if (timer >= secondsToDeath) {
        Game::Instance().RequestPop();
        Game::Instance().RequestPush(new EndState{});
    }
}

void EndStateDimmer::Render(Vec2<Cart>) {
    float opacity = 255.0f * timer / secondsToDeath;

    auto renderer = Game::Instance().Renderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);  // ?
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, std::round(opacity));
    SDL_Rect rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
}

bool EndStateDimmer::Is(CType type) { return type == CType::EndStageDimmer; }
