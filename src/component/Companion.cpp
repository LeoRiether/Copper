#include "component/Companion.h"

#include "InputManager.h"
#include "Prefabs.h"
#include "component/Animation.h"
#include "component/Player.h"
#include "component/enemy/RobotCan.h"
#include "math/Direction.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "Companion"

Companion::Companion(GameObject& go) : Component(go) {
    associated.tags.set(tag::Entity);
}

void Companion::Update(float dt) {
    if (!Player::player) return;  // *sad companion noises*

    updatePosition(dt);
    updateCore(dt);
    updateState(dt);
}

void Companion::updatePosition(float dt) {
    auto playerPos = Player::player->Associated().box.Foot();
    auto selfPos = associated.box.Foot();
    auto distVec = playerPos - selfPos;

    // If too far away, just move companion close really fast
    if (distVec.norm2() >= 40000) {
        associated.box.OffsetBy(distVec * 0.01);
    }

    auto rc = (RobotCan*)associated.GetComponent(CType::RobotCan);
    if (!rc) {
        warn("no associated RobotCan!");
        return;
    }

    if (distVec.norm() > stopDistance) {
        rc->direction = Direction::approxFromVec(distVec);
        baseAnimPlay("walk_" + rc->direction.toString());
        rc->associated.box.OffsetBy(distVec.normalize() * speed * dt);
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
            auto mouseDelta = input.Mouse() - associated.box.Center();
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
        if (input.MousePress(1)) {
            auto mouseDelta = input.Mouse() - associated.box.Center();
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
