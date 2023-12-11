#include "component/Companion.h"

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "InputManager.h"
#include "Prefabs.h"
#include "SDL_render.h"
#include "component/Animation.h"
#include "component/IsoCollider.h"
#include "component/Player.h"
#include "component/enemy/RobotCan.h"
#include "math/Direction.h"
#include "physics/Steering.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "Companion"

Companion::Companion(GameObject& go) : Component(go) {
    associated.tags.set(tag::Entity);
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
    auto rc = (RobotCan*)associated.GetComponent(CType::RobotCan);
    if (!rc) {
        warn("no associated RobotCan!");
        return;
    }

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
        allAnimsPlay("hide_" + rc->direction.toString(), false);
        return;
    }

    moveDelta = (*maybePlayerPos - selfPos).toCart().normalize() +
                Steering{}.AddTerrain(selfPos)->Result().toCart();
    moveDelta = moveDelta.normalize();

    if (realDistVec.norm2() > stopDistance * stopDistance) {
        rc->direction = Direction::approxFromVec(moveDelta);
        baseAnimPlay("walk_" + rc->direction.toString());
        associated.box.OffsetBy(moveDelta * speed * dt);
        walkingToIdleTimeout.Restart();
    } else {
        walkingToIdleTimeout.Update(dt);
        if (walkingToIdleTimeout.Get() >= 0.1)
            baseAnimPlay("idle_" + rc->direction.toString());
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
            coreAnimPlay("fire1_" + dir.toString(), false);
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
