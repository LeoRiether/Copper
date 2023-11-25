#include "component/IsoCollider.h"

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "component/Sprite.h"
#include "math/Vec2.h"
#include "util.h"

#define MODULE "IsoCollider"

IsoCollider::IsoCollider(GameObject& associated) : Component(associated) {}

void IsoCollider::Update(float) {
    box.w = base.w;
    box.h = base.h;

    Vec2<Iso> something = associated.box.TopLeft().toIso();
    box.x = base.x + something.x;
    box.y = base.y + something.y;
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

IsoCollider* IsoCollider::ScaleToSprite() {
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

IsoCollider* IsoCollider::ExpandBy(float pixels) {
    base.x -= pixels;
    base.y -= pixels;
    base.w += 2 * pixels;
    base.h += 2 * pixels;
    return this;
}

IsoCollider* IsoCollider::WithBase(Rect b) {
    base = b;
    return this;
}

bool IsoCollider::Is(CType type) { return type == CType::IsoCollider; }
