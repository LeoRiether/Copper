#include "component/IsoCollider.h"

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "IsoCollider"

IsoCollider::IsoCollider(GameObject& associated) : Component(associated) {}

void IsoCollider::Update(float) {
    box.w = offset.w;
    box.h = offset.h;

    Vec2<Iso> base = associated.box.TopLeft().toIso();
    box.x = base.x + offset.x;
    box.y = base.y + offset.y;
}

void IsoCollider::Render(Vec2<Cart> camera) {
    static int& showColliders = Consts::GetInt("debug.show_colliders");

    if (showColliders) {
        SDL_Point points[5];

        Vec2<Cart> point = Vec2<Iso>{box.x, box.y}.toCart() - camera;
        points[0] = {(int)point.x, (int)point.y};
        points[4] = {(int)point.x, (int)point.y};

        point = Vec2<Iso>{box.x + box.w, box.y}.toCart() - camera;
        points[1] = {(int)point.x, (int)point.y};

        point = Vec2<Iso>{box.x + box.w, box.y + box.h}.toCart() - camera;
        points[2] = {(int)point.x, (int)point.y};

        point = Vec2<Iso>{box.x, box.y + box.h}.toCart() - camera;
        points[3] = {(int)point.x, (int)point.y};

        SDL_SetRenderDrawColor(Game::Instance().Renderer(), 0, 255, 0,
                               SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLines(Game::Instance().Renderer(), points, 5);
    }
}

void IsoCollider::ScaleToSprite() {
    auto sprite = (Sprite*)associated.GetComponent(CType::Sprite);
    if (!sprite) {
        fail("no Sprite component found");
    }
    offset.x *= sprite->Scale().x;
    offset.y *= sprite->Scale().y;
    offset.w *= sprite->Scale().x;
    offset.h *= sprite->Scale().y;
}

void IsoCollider::ExpandBy(float pixels) {
    offset.x -= pixels;
    offset.y -= pixels;
    offset.w += 2 * pixels;
    offset.h += 2 * pixels;
}

bool IsoCollider::Is(CType type) { return type == CType::IsoCollider; }
