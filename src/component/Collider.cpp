#include "component/Collider.h"

#include "CType.h"
#include "Consts.h"
#include "Game.h"

Collider::Collider(GameObject& associated) : Component(associated) {}

void Collider::Update(float) {
    if (base.w == 0 && base.h == 0) {
        box = associated.box;
    } else {
        box = base;
        box.x += associated.box.x;
        box.y += associated.box.y;
    }
}

void Collider::Render(Vec2<Cart> camera) {
    static int& showColliders = Consts::GetInt("debug.show_colliders");

    if (showColliders) {
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
    }
}

bool Collider::Is(CType type) { return type == CType::Collider; }

Collider* Collider::WithBase(Rect b) {
    base = b;
    return this;
}
