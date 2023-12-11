#include "component/StageTransitionDimmer.h"

#include "Game.h"
#include "SDL_include.h"
#include "math/Interpolate.h"

#define MODULE "StageTransitionDimmer"

StageTransitionDimmer::StageTransitionDimmer(GameObject& go, float from,
                                             float to, float duration)
    : Component(go), from(from), to(to), duration(duration) {
    associated.renderLayer = 12345678;
}

StageTransitionDimmer* StageTransitionDimmer::WithEasing(Easing e) {
    easing = e;
    return this;
}

void StageTransitionDimmer::Update(float dt) {
    now += dt;
    if (now >= duration + 1.0) {
        associated.RequestDelete();
    }
}

void StageTransitionDimmer::Render(Vec2<Cart>) {
    float p = std::min(1.0f, now / duration);
    float alpha = easing == In ? qerp_in(from, to, p) : qerp_out(from, to, p);

    auto renderer = Game::Instance().Renderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(renderer, alpha, alpha, alpha, std::round(alpha));
    SDL_Rect rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
}
