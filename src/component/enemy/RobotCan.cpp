#include "component/enemy/RobotCan.h"

#include "CType.h"
#include "Camera.h"
#include "Consts.h"
#include "Game.h"
#include "GameData.h"
#include "Prefabs.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/OverheadHpBar.h"
#include "component/Player.h"
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

void RobotCan::Start() {
    Game::Instance().GetState().GetCamera().SecondaryFollow(associated.weak);
}

void RobotCan::Update(float dt) {
    associated.box.OffsetBy(knockbackVelocity * dt);
    knockbackVelocity = knockbackVelocity * 0.70;
    stunnedLevel = std::max<float>(0.0, stunnedLevel - dt);
}

void RobotCan::Render(Vec2<Cart>) {}

void RobotCan::NotifyCollisionEnter(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    bool bulletHit = bullet && !bullet->TargetsPlayer();
    bool meleeHit = other.tags.test(tag::PlayerHitbox);
	bool explosion = other.tags.test(tag::Explosion);
	int damage = explosion ? 75 : 25;
    if (bulletHit || meleeHit) {

        // Player stops loosing HP
        Player::player->hpLoss = 0;
        Player::player->hpLossTimer.Restart();

        auto bar =
            (OverheadHpBar*)associated.GetComponent(CType::OverheadHpBar);
        if (bar) {
            bar->SetHp(bar->Hp() - damage);
            associated.RequestAdd(
                MakeHitMarker(damage)->WithFootAt(associated.box.Head()));
        }

        if (bar && bar->Hp() <= 0) {
            Die();
			if (!explosion)
				other.RequestDelete();
            return;
        }

        // Trauma
        if (meleeHit) {
            Game::Instance().AddTrauma(0.3);
            Game::Instance().Slowdown(0.03, 0.2);
            stunnedLevel += 1.0f;
        }

        // Flash
        for (auto& sprite : associated.GetAllComponents(CType::Sprite)) {
            ((Sprite*)sprite.get())->WithFlash(0.08);
        }

        // Explosion
        auto hitpoint = other.box.Center();
        hitpoint = hitpoint + Vec2<Cart>{25, 0}.GetRotated(other.angle);
        associated.RequestAdd(MakeExplosion1()->WithCenterAt(hitpoint));

        // Knockback
        float kb = 150'000 * Game::Instance().DeltaTime();
        knockbackVelocity = Vec2<Cart>{kb, 0}.GetRotated(other.angle);

		if (explosion)
				return;
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
    static int& powerupChance = Consts::GetInt("powerup.chance");
    auto center = associated.box.Center();
    associated.RequestAdd(MakeExplosion4()->WithCenterAt(center));
    associated.RequestDelete();
    if (randi(0, 99) < powerupChance)
        associated.RequestAdd(MakeRandomPowerup()->WithCenterAt(center));

	int coins = randi(0, 5);
	for (int i = 0; i < coins; i++) {
		associated.RequestAdd(MakeCoin()
			->WithCenterAt(center +
				Vec2<Cart>{randi(0,30), randi(0,30)}));
	}

    GameData::enemiesKilled++;
}
