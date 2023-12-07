#include "component/Collider.h"

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "util.h"

#define MODULE "Collider"

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

Collider* Collider::WithBase(Rect b) {
    base = b;
    return this;
}

Collider* Collider::WithTag(int tag) {
    tags.set(tag);
    return this;
}

Collider* Collider::ScaleToSprite() {
    auto sprite = (Sprite*)associated.GetComponent(CType::Sprite);
    if (!sprite) {
        fail("no Sprite component found");
    }
    base.x *= sprite->Scale().x;
    base.y *= sprite->Scale().y;
    base.w *= sprite->Scale().x;
    base.h *= sprite->Scale().y;
    return this;
}
