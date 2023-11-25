#include "component/enemy/RobotCan.h"

#include "Game.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Sprite.h"
#include "math/Direction.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "RobotCan"

RobotCan::RobotCan(GameObject& associated) : Component(associated) {
    associated.tags.set(tag::Entity);

    auto baseSprite = new Sprite{associated, ASSETS "/img/RobotCanBase.png"};
    associated.AddComponent(baseSprite);

    auto coreSprite = new Sprite{associated, ASSETS "/img/RobotCanCore.png"};
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
            {"walk_S", 10, 0.1},   {"walk_SW", 10, 0.1},
            {"walk_W", 10, 0.1},   {"walk_NW", 10, 0.1},
            {"walk_N", 10, 0.1},   {"walk_NE", 10, 0.1},
            {"walk_E", 10, 0.1},   {"walk_SE", 10, 0.1},
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

void RobotCan::Update(float) {}

void RobotCan::Render(Vec2<Cart>) {}

bool RobotCan::Is(CType type) { return type == CType::RobotCan; }

void RobotCan::NotifyCollision(GameObject& other) {
    if (other.GetComponent(CType::Bullet)) {
        warn("RobotCan hit!");
        other.RequestDelete();
    }
}

RobotCan* RobotCan::WithStopDistance(float value) {
    stopDistance = value;
    return this;
}
