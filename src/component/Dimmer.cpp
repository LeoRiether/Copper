#include "component/Dimmer.h"

#include "Component.h"
#include "Game.h"

#define MODULE "Dimmer"

Dimmer::Dimmer(GameObject &associated) : Component(associated) {}

void Dimmer::Render(Vec2<Cart>) {
  auto renderer = Game::Instance().Renderer();
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // ?
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
  SDL_Rect rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  SDL_RenderFillRect(renderer, &rect);
}

