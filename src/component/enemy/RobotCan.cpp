#include "component/enemy/RobotCan.h"

#include "CType.h"
#include "Camera.h"
#include "Game.h"
#include "Prefabs.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/OverheadHpBar.h"
#include "component/Sprite.h"
#include "math/Direction.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "RobotCan"

RobotCan::RobotCan(GameObject& associated) : Component(associated) {
    associated.tags.set(tag::Entity);

    auto baseSprite = new Sprite{associated, ASSETS "/img/RobotCanBase.png"};
    baseSprite->SetHasShadow(true);
    associated.AddComponent(baseSprite);

    auto coreSprite = new Sprite{associated, ASSETS "/img/RobotCanCore.png"};
    coreSprite->SetHasShadow(true);
    associated.AddComponent(coreSprite);

    // Animation
    {
        auto coreAnim = new Animation{associated, *coreSprite};
        GridKeyframe coreGrid{21, 32, coreSprite->SheetWidth(),
                              coreSprite->SheetHeight(), 0.1};

        auto baseAnim = new Animation{associated, *baseSprite};
        GridKeyframe baseGrid{21, 32, baseSprite->SheetWidth(),
                              baseSprite->SheetHeight(), 0.1};

        auto row = [&](GridKeyframe& grid, int i, int startJ, int frames,
                       float frameTime = 0.1) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++) {
                kf.push_back(grid.At(j, i));
                kf.back().frameTime = frameTime;
            }
            return kf;
        };

        struct item_t {
            const char* id;
            int columns;
            float frameTime;
        };
        constexpr item_t items[] = {
            {"walk_S", 10, 0.05},  {"walk_SW", 10, 0.05},
            {"walk_W", 10, 0.05},  {"walk_NW", 10, 0.05},
            {"walk_N", 10, 0.05},  {"walk_NE", 10, 0.05},
            {"walk_E", 10, 0.05},  {"walk_SE", 10, 0.05},
            {"fire1_S", 5, 0.1},   {"fire1_SW", 5, 0.1},
            {"fire1_W", 5, 0.1},   {"fire1_NW", 5, 0.1},
            {"fire1_N", 5, 0.1},   {"fire1_NE", 5, 0.1},
            {"fire1_E", 5, 0.1},   {"fire1_SE", 5, 0.1},
            {"fire2_S", 21, 0.05}, {"fire2_SW", 21, 0.05},
            {"fire2_W", 21, 0.05}, {"fire2_NW", 21, 0.05},
            {"fire2_N", 21, 0.05}, {"fire2_NE", 21, 0.05},
            {"fire2_E", 21, 0.05}, {"fire2_SE", 21, 0.05},
            {"hide_S", 11, 0.1},   {"hide_SW", 11, 0.1},
            {"hide_W", 11, 0.1},   {"hide_NW", 11, 0.1},
            {"hide_N", 11, 0.1},   {"hide_NE", 11, 0.1},
            {"hide_E", 11, 0.1},   {"hide_SE", 11, 0.1},
        };
        constexpr int n = sizeof(items) / sizeof(*items);

        for (int i = 0; i < n; i++) {
            const auto& item = items[i];
            coreAnim->AddKeyframes(
                item.id, row(coreGrid, i, 0, item.columns, item.frameTime));
            baseAnim->AddKeyframes(
                item.id, row(baseGrid, i, 0, item.columns, item.frameTime));
        }

        constexpr const char* idles[] = {"idle_S",  "idle_SW", "idle_W",
                                         "idle_NW", "idle_N",  "idle_NE",
                                         "idle_E",  "idle_SE"};
        for (int i = 0; i < 8; i++) {
            coreAnim->AddKeyframes(idles[i], row(coreGrid, i, 0, 1, 1.0));
            baseAnim->AddKeyframes(idles[i], row(baseGrid, i, 0, 1, 1.0));
        }

        baseAnim->Play("walk_S");  // just to kickstart the associated.box...
        associated.AddComponent(coreAnim);
        associated.AddComponent(baseAnim);
    }

    direction = Direction{NoneX, Down};
}

void RobotCan::Update(float dt) {
    associated.box.OffsetBy(knockbackVelocity * dt);
    knockbackVelocity = knockbackVelocity * 0.70;

    flashTimeout -= dt;
    if (flashTimeout <= 0) {
        flashTimeout = INFINITY;  // won't trigger this part again very soon
        for (auto& sprite : associated.GetAllComponents(CType::Sprite)) {
            ((Sprite*)sprite.get())->WithFlash(false);
        }
    }

    stunnedLevel = std::max<float>(0.0, stunnedLevel - dt);
}

void RobotCan::Render(Vec2<Cart>) {}

void RobotCan::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    bool isCompanion = associated.GetComponent(CType::Companion) != nullptr;

    bool bulletHit = bullet && bullet->TargetsPlayer() == isCompanion;
    bool meleeHit = other.tags.test(tag::PlayerHitbox);
    if (bulletHit || meleeHit) {
        if (!isCompanion) {
            auto bar =
                (OverheadHpBar*)associated.GetComponent(CType::OverheadHpBar);
            if (bar) {
                bar->SetHp(bar->Hp() - 25);
            }

            // Trauma
            if (meleeHit) {
                Game::Instance().AddTrauma(0.3);
                Game::Instance().Slowdown(0.03, 0.2);
                stunnedLevel += 1.0f;
            }

            Game::Instance().GetState().GetCamera().SecondaryFollow(
                associated.weak);

            if (bar && bar->Hp() <= 0) {
                Die();
                other.RequestDelete();
                return;
            }
        }

        // Flash
        for (auto& sprite : associated.GetAllComponents(CType::Sprite)) {
            ((Sprite*)sprite.get())->WithFlash(true);
        }
        flashTimeout = 0.08;

        // Explosion
        auto hitpoint = other.box.Center();
        hitpoint = hitpoint + Vec2<Cart>{25, 0}.GetRotated(other.angle);
        associated.RequestAdd(MakeExplosion1()->WithCenterAt(hitpoint));

        // Knockback
        float kb = 80000 * Game::Instance().DeltaTime();
        knockbackVelocity = Vec2<Cart>{kb, 0}.GetRotated(other.angle);

        other.RequestDelete();
    }
}

RobotCan* RobotCan::WithStopDistance(float value) {
    stopDistance = value;
    return this;
}

RobotCan* RobotCan::WithHp(int hp) {
    auto bar = (OverheadHpBar*)associated.GetComponent(CType::OverheadHpBar);
    if (!bar) fail("WithHp called, but no associated OverheadHpBar");
    bar->SetHp(hp);
    return this;
}

void RobotCan::Die() {
    auto center = associated.box.Center();
    associated.RequestAdd(MakeExplosion4()->WithCenterAt(center));
    associated.RequestDelete();
}
