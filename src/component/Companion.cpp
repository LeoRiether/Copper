#include "component/Companion.h"

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "InputManager.h"
#include "Prefabs.h"
#include "SDL_render.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/IsoCollider.h"
#include "component/Player.h"
#include "math/Direction.h"
#include "physics/Steering.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "Companion"

Companion::Companion(GameObject& go) : Component(go) {
    associated.tags.set(tag::Entity);

    auto baseSprite = new Sprite{associated, ASSETS "/img/Companion/base.png"};
    baseSprite->SetHasShadow(true);
    associated.AddComponent(baseSprite);

    auto coreSprite = new Sprite{associated, ASSETS "/img/Companion/core.png"};
    coreSprite->SetHasShadow(true);
    associated.AddComponent(coreSprite);

    // Animation
    {
        auto coreAnim = new Animation{associated, *coreSprite};
        GridKeyframe coreGrid{20, 16, coreSprite->SheetWidth(),
                              coreSprite->SheetHeight(), 0.1};

        auto baseAnim = new Animation{associated, *baseSprite};
        GridKeyframe baseGrid{20, 16, baseSprite->SheetWidth(),
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
            {"walk_S", 20, 0.05},  {"walk_SW", 20, 0.05}, {"walk_W", 20, 0.05},
            {"walk_NW", 20, 0.05}, {"walk_N", 20, 0.05},  {"walk_NE", 20, 0.05},
            {"walk_E", 20, 0.05},  {"walk_SE", 20, 0.05}, {"fire_S", 5, 0.1},
            {"fire_SW", 5, 0.1},   {"fire_W", 5, 0.1},    {"fire_NW", 5, 0.1},
            {"fire_N", 5, 0.1},    {"fire_NE", 5, 0.1},   {"fire_E", 5, 0.1},
            {"fire_SE", 5, 0.1},
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
}

//////////////////////////////////////////
//        Update-related methods        //
//////////////////////////////////////////
void Companion::Update(float dt) {
    if (!Player::player) return;  // *sad companion noises*

    updatePosition(dt);
    updateCore(dt);
    updateState(dt);
}

void Companion::updatePosition(float dt) {
    auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
    if (!iso) fail("companion without IsoCollider...");
    auto selfPos = iso->box.Center().transmute<Iso>();
    auto maybePlayerPos = Player::player->LookForMe(iso->box);

    // Distance to the real player, not some trail
    auto realPlayerPos = Player::player->associated.box.Center();
    auto realDistVec = realPlayerPos - selfPos.toCart();

    // If too far away, just move companion close really fast
    if (realDistVec.norm2() >= 80000) {
        associated.box.OffsetBy(realDistVec * 0.01);
        tooFarTimer.Update(dt);
        if (tooFarTimer.Get() >= 3.0) {
            associated.box.OffsetBy(realDistVec * 0.3);
        }
    } else {
        tooFarTimer.Restart();
    }

    if (!maybePlayerPos) {
        return;
    }

    moveDelta = (*maybePlayerPos - selfPos).toCart().normalize() +
                Steering{}.AddTerrain(selfPos)->Result().toCart();
    moveDelta = moveDelta.normalize();

    if (realDistVec.norm2() > stopDistance * stopDistance) {
        direction = Direction::approxFromVec(moveDelta);
        baseAnimPlay("walk_" + direction.toString());
        associated.box.OffsetBy(moveDelta * speed * dt);
        walkingToIdleTimeout.Restart();
    } else {
        walkingToIdleTimeout.Update(dt);
        if (walkingToIdleTimeout.Get() >= 0.1)
            baseAnimPlay("idle_" + direction.toString());
    }
}

void Companion::updateCore(float) {
    auto& input = InputManager::Instance();

    switch (state) {
        case Looking: {
            auto mouseDelta = input.HasController()
                                  ? input.AxisVec(1)
                                  : input.Mouse() - associated.box.Center();
            auto lookingDir = Direction::approxFromVec(mouseDelta);
            coreAnimPlay("walk_" + lookingDir.toString());
            break;
        }
        case Firing: {
            break;
        }
    }
}

void Companion::updateState(float dt) {
    auto& input = InputManager::Instance();

    auto checkShootInput = [&]() {
        if (input.MousePress(3) ||
            input.ControllerPress(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
            auto mouseDelta = input.HasController()
                                  ? input.AxisVec(1)
                                  : input.Mouse() - associated.box.Center();
            associated.RequestAdd(
                MakePlayerBullet(associated.box.Center(), mouseDelta.angle()));
            auto dir = Direction::approxFromVec(mouseDelta);
            coreAnimPlay("fire_" + dir.toString(), false);
            changeState(Firing);
        }
    };

    switch (state) {
        case Looking: {
            checkShootInput();
            break;
        }
        case Firing: {
            firingTimeout.Update(dt);
            if (firingTimeout.Get() >= firingTimeoutSecs) {
                changeState(Looking);
            }
            break;
        }
    }
}

void Companion::changeState(State newState) {
    switch (newState) {
        case Looking: {
            break;
        }
        case Firing: {
            firingTimeout.Restart();
            break;
        }
    }

    state = newState;
}

void Companion::allAnimsPlay(const string& id, bool loops) {
    auto& anims = associated.GetAllComponents(CType::Animation);
    for (auto& anim : anims) {
        ((Animation*)anim.get())->SoftPlay(id, loops);
    }
}

void Companion::coreAnimPlay(const string& id, bool loops) {
    auto coreAnim = (Animation*)associated.GetComponent(CType::Animation);
    if (!coreAnim) fail("no core Animation found!");
    coreAnim->SoftPlay(id, loops);
}

void Companion::baseAnimPlay(const string& id, bool loops) {
    auto& anims = associated.GetAllComponents(CType::Animation);
    if (anims.size() < 2) fail("no base Animation found!");
    ((Animation*)anims[1].get())->SoftPlay(id, loops);
}

//////////////////////////
//        Render        //
//////////////////////////
void Companion::Render(Vec2<Cart> camera) {
    auto renderer = Game::Instance().Renderer();

    static int& showDirections = Consts::GetInt("debug.show_directions");
    if (showDirections) {
        auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
        if (!iso) fail("no IsoCollider");
        auto from = iso->box.Center().transmute<Iso>().toCart() - camera;
        auto to = from + moveDelta * 80.0f;
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
    }

    InputManager& input = InputManager::Instance();
    if (input.HasController()) {
        auto pos = associated.box.Center() - camera + input.AxisVec(1) * 120.0f;
        SDL_FRect rects[2] = {{pos.x - 1, pos.y - 7, 2, 14},
                              {pos.x - 7, pos.y - 1, 14, 2}};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_RenderFillRectsF(renderer, rects, 2);
    }
}

///////////////////////////////////
//        NotifyCollision        //
///////////////////////////////////
void Companion::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    bool bulletHit = bullet && bullet->TargetsPlayer();
	bool explosion = other.tags.test(tag::Explosion);
    if (bulletHit) {
        // Flash
        for (auto& sprite : associated.GetAllComponents(CType::Sprite)) {
            ((Sprite*)sprite.get())->WithFlash(0.08);
        }

        // Explosion
        auto hitpoint = other.box.Center();
        hitpoint = hitpoint + Vec2<Cart>{25, 0}.GetRotated(other.angle);
        associated.RequestAdd(MakeExplosion1()->WithCenterAt(hitpoint));
        
		if (explosion)
				return;
        other.RequestDelete();
    }
}
