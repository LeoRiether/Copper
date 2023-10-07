#include "component/Enemy.h"

#include "Collider.h"
#include "component/Animation.h"
#include "component/Player.h"
#include "component/Sprite.h"
#include "math/Direction.h"
#include "util.h"

#define MODULE "Enemy"

Enemy::Enemy(GameObject& associated) : Component(associated) {
    auto sprite = new Sprite{associated, ASSETS "/img/spaghetti_atlas.png"};
    sprite->SetHasShadow(true);
    associated.AddComponent(sprite);

    {
        auto anim = new Animation{associated};
        GridKeyframe grid{24, 12, sprite->SheetWidth(), sprite->SheetHeight(),
                          0.1};

        auto row = [&](int i, int startJ, int frames) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++)
                kf.push_back(grid.At(j, i));
            return kf;
        };

        anim->AddKeyframes("E", row(0, 0, 6));
        anim->AddKeyframes("NE", row(0, 6, 6));
        anim->AddKeyframes("N", row(0, 12, 6));
        anim->AddKeyframes("NW", row(0, 18, 6));
        anim->AddKeyframes("W", row(1, 0, 6));
        anim->AddKeyframes("SW", row(1, 6, 6));
        anim->AddKeyframes("SE", row(1, 12, 6));
        anim->AddKeyframes("S", row(1, 18, 6));
        anim->AddKeyframes("idle_E", row(0, 0, 1));
        anim->AddKeyframes("idle_NE", row(0, 6, 1));
        anim->AddKeyframes("idle_N", row(0, 12, 1));
        anim->AddKeyframes("idle_NW", row(0, 18, 1));
        anim->AddKeyframes("idle_W", row(1, 0, 1));
        anim->AddKeyframes("idle_SW", row(1, 6, 1));
        anim->AddKeyframes("idle_SE", row(1, 12, 1));
        anim->AddKeyframes("idle_S", row(1, 18, 1));
        anim->Play("idle_S");  // just to kickstart the associated.box...
        associated.AddComponent(anim);
    }

    associated.AddComponent(new Collider{associated});
    direction = Direction{NoneX, Down};
}

void Enemy::Update(float dt) {
    if (!Player::player) return;

    auto anim = (Animation*)associated.GetComponent(CType::Animation);
    auto playerPos = Player::player->Associated().box.Foot();
    auto enemyPos = associated.box.Foot();
    auto distVec = playerPos - enemyPos;
    if (distVec.norm() <= 100) {
        anim->SoftPlay("idle_" + direction.toString());
        return;
    }

    direction = Direction::approxFromVec(distVec);
    anim->SoftPlay(direction.toString());
    associated.box.OffsetBy(distVec.normalize() * 300 * dt);
}

void Enemy::Render(Vec2<Cart>) {}

bool Enemy::Is(CType type) { return type == CType::Enemy; }
