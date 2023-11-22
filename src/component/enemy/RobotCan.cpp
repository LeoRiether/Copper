#include "component/enemy/RobotCan.h"

#include <algorithm>

#include "Game.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Collider.h"
#include "component/Player.h"
#include "component/Sprite.h"
#include "math/Direction.h"
#include "util.h"

#define MODULE "RobotCan"

RobotCan::RobotCan(GameObject& associated) : Component(associated) {
    associated.tags.set(tag::Entity);

    auto baseSprite = new Sprite{
        associated, ASSETS "/img/RobotCan/RobotCanWalkBase/spritesheet.png"};
    associated.AddComponent(baseSprite);

    auto coreSprite = new Sprite{
        associated, ASSETS "/img/RobotCan/RobotCanWalkCore/spritesheet.png"};
    associated.AddComponent(coreSprite);

    // Base animation
    {
        auto anim = new Animation{associated, *baseSprite};
        GridKeyframe grid{10, 8, coreSprite->SheetWidth(),
                          coreSprite->SheetHeight(), 0.1};

        auto row = [&](int i, int startJ, int frames) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++)
                kf.push_back(grid.At(j, i));
            return kf;
        };

        anim->AddKeyframes("S", row(0, 0, 10));
        anim->AddKeyframes("SW", row(1, 0, 10));
        anim->AddKeyframes("W", row(2, 0, 10));
        anim->AddKeyframes("NW", row(3, 0, 10));
        anim->AddKeyframes("N", row(4, 0, 10));
        anim->AddKeyframes("NE", row(5, 0, 10));
        anim->AddKeyframes("E", row(6, 0, 10));
        anim->AddKeyframes("SE", row(7, 0, 10));
        anim->AddKeyframes("idle_S", row(0, 0, 1));
        anim->AddKeyframes("idle_SW", row(1, 0, 1));
        anim->AddKeyframes("idle_W", row(2, 0, 1));
        anim->AddKeyframes("idle_NW", row(3, 0, 1));
        anim->AddKeyframes("idle_N", row(4, 0, 1));
        anim->AddKeyframes("idle_NE", row(5, 0, 1));
        anim->AddKeyframes("idle_E", row(6, 0, 1));
        anim->AddKeyframes("idle_SE", row(7, 0, 1));
        anim->Play("SE");  // just to kickstart the associated.box...
        associated.AddComponent(anim);
    }

    // Core animation
    {
        auto anim = new Animation{associated, *coreSprite};
        GridKeyframe grid{10, 8, coreSprite->SheetWidth(),
                          coreSprite->SheetHeight(), 0.1};

        auto row = [&](int i, int startJ, int frames) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++)
                kf.push_back(grid.At(j, i));
            return kf;
        };

        anim->AddKeyframes("S", row(0, 0, 10));
        anim->AddKeyframes("SW", row(1, 0, 10));
        anim->AddKeyframes("W", row(2, 0, 10));
        anim->AddKeyframes("NW", row(3, 0, 10));
        anim->AddKeyframes("N", row(4, 0, 10));
        anim->AddKeyframes("NE", row(5, 0, 10));
        anim->AddKeyframes("E", row(6, 0, 10));
        anim->AddKeyframes("SE", row(7, 0, 10));
        anim->AddKeyframes("idle_S", row(0, 0, 1));
        anim->AddKeyframes("idle_SW", row(1, 0, 1));
        anim->AddKeyframes("idle_W", row(2, 0, 1));
        anim->AddKeyframes("idle_NW", row(3, 0, 1));
        anim->AddKeyframes("idle_N", row(4, 0, 1));
        anim->AddKeyframes("idle_NE", row(5, 0, 1));
        anim->AddKeyframes("idle_E", row(6, 0, 1));
        anim->AddKeyframes("idle_SE", row(7, 0, 1));
        anim->Play("SE");  // just to kickstart the associated.box...
        associated.AddComponent(anim);
    }

    direction = Direction{NoneX, Down};
}

void RobotCan::Update(float dt) {
    if (!behavior) {
        warn("unset behavior!");
        return;
    }

    behavior->Update(*this, dt);

    if (bulletTimer.Get() >= BULLET_DELAY) {
        bulletTimer.Restart();

        const auto playerPos = Player::player->Associated().box.Center();
        const auto angle = (playerPos - associated.box.Center()).angle();

        auto go = new GameObject{};
        auto sprite = new Sprite{*go, ASSETS "/img/laser-bullet.png"};
        go->AddComponent(new Bullet{*go, 700, angle, 20, 1000, true});
        go->AddComponent(sprite);
        go->AddComponent(
            (new Collider{*go})
                ->WithBase(Rect{46.555, 55.1247, 28.3535, 11.9118}));
        go->box = Rect{0, 0, (float)sprite->SheetWidth(),
                       (float)sprite->SheetHeight()};
        go->box.SetCenter(associated.box.Center());
        Game::Instance().GetState().RequestAddObject(go);
    }
}

void RobotCan::Render(Vec2<Cart>) {}

bool RobotCan::Is(CType type) { return type == CType::RobotCan; }

RobotCan& RobotCan::WithStopDistance(float value) {
    stopDistance = value;
    return *this;
}

RobotCan& RobotCan::WithBehavior(EnemyBehavior* value) {
    behavior = unique_ptr<EnemyBehavior>(value);
    return *this;
}
