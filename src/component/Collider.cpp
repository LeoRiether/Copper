#include "Collider.h"

#include "CType.h"
#include "Game.h"

Collider::Collider(GameObject& associated, Vec2<Cart> scale, Vec2<Cart> offset)
    : Component(associated), scale(scale), offset(offset) {}

void Collider::Update(float) {
    box = associated.box;

    box.w *= scale.x;
    box.h *= scale.y;

    offset = offset.GetRotated(associated.angle);
    box.x += offset.x;
    box.y += offset.y;
}

void Collider::Render(Vec2<Cart> camera) {
#ifdef DEBUG
    Vec2<Cart> center(box.Center());
    SDL_Point points[5];

    Vec2<Cart> point =
        (Vec2<Cart>{box.x, box.y} - center).GetRotated(associated.angle) +
        center - camera;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = (Vec2<Cart>{box.x + box.w, box.y} - center)
                .GetRotated(associated.angle) +
            center - camera;
    points[1] = {(int)point.x, (int)point.y};

    point = (Vec2<Cart>{box.x + box.w, box.y + box.h} - center)
                .GetRotated(associated.angle) +
            center - camera;
    points[2] = {(int)point.x, (int)point.y};

    point = (Vec2<Cart>{box.x, box.y + box.h} - center)
                .GetRotated(associated.angle) +
            center - camera;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::Instance().Renderer(), 255, 0, 0,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::Instance().Renderer(), points, 5);
#endif  // DEBUG
}

bool Collider::Is(CType type) { return type == CType::Collider; }

void Collider::SetScale(Vec2<Cart> s) { scale = s; }
void Collider::SetOffset(Vec2<Cart> o) { offset = o; }
