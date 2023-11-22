#include "component/enemy/RobotCan.h"

#include <cstring>

#include "component/Animation.h"
#include "component/Collider.h"
#include "component/Player.h"
#include "math/Direction.h"
#include "util.h"

#define MODULE "RobotCan"

RobotCan::RobotCan(GameObject& associated) : Component(associated) {
    baseSprite = new Sprite{
        associated, ASSETS "/img/RobotCan/RobotCanWalkBase/spritesheet.png"};
    associated.AddComponent(baseSprite);

    coreSprite = new Sprite{
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

    associated.AddComponent(new Collider{associated});
}

void RobotCan::Update(float) {
    auto player = Player::player;
    if (player != nullptr) {
        auto dir = Direction::approxFromVec(player->associated.box.Center() -
                                            associated.box.Center());

        auto anims = associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim)->SoftPlay(dir.toString());
        }
    }
}

bool RobotCan::Is(CType type) { return type == CType::RobotCan; }
