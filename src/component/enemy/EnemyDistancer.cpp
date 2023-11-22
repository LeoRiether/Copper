#include "component/enemy/EnemyDistancer.h"

#include "Game.h"
#include "GameObject.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Player.h"
#include "component/enemy/RobotCan.h"

EnemyDistancer::EnemyDistancer(GameObject& go) : Component(go) {}

EnemyDistancer* EnemyDistancer::WithRobotCan(RobotCan* rc) {
    self = rc;
    return this;
}

void EnemyDistancer::Update(float dt) {
    if (!Player::player) return;

    auto walkAway = [&]() {
        auto playerPos = Player::player->Associated().box.Foot();
        auto enemyPos = associated.box.Foot();
        auto distVec = enemyPos - playerPos;
        self->direction = Direction::approxFromVec(distVec);

        const float stop2 = self->stopDistance * self->stopDistance;
        if (distVec.norm2() > stop2 &&
            (walkingTime >= 0.3 || walkingTime == 0)) {
            allAnimsPlay("idle_" + (-self->direction).toString());
            walkingTime = 0;
            return;
        }

        allAnimsPlay(self->direction.toString());
        associated.box.OffsetBy(distVec.normalize() * 300 * dt);
        walkingTime += dt;
    };

    switch (state) {
        case WalkingAway: {
            walkAway();

            timeBetweenShots.Update(dt);
            if (timeBetweenShots.Get() >= timeBetweenShotsConst) {
                switchState(Shooting);
            }

            break;
        }
        case Shooting: {
            shotTimeout.Update(dt);
            if (shotTimeout.Get() >= bulletDelay) {
                switchState(WalkingAway);
            }
            break;
        }
    }
}

void EnemyDistancer::switchState(State newState) {
    // Get out of old state
    switch (state) {
        case WalkingAway: {
            break;
        }
        case Shooting: {
            break;
        }
    }

    auto shoot = [&]() {
        const auto playerPos = Player::player->Associated().box.Foot();
        const auto angle = (playerPos - associated.box.Center()).angle();
        auto go = Bullet::Make(associated.box.Center(), angle);
        Game::Instance().GetState().RequestAddObject(go);

        // TODO: switch animation
    };

    // Get in new state
    switch (newState) {
        case WalkingAway: {
            timeBetweenShots.Restart();
            walkingTime = 0;
            break;
        }
        case Shooting: {
            shoot();
            shotTimeout.Restart();
            break;
        }
    }

    state = newState;
}

void EnemyDistancer::allAnimsPlay(const string& id) {
    auto anims = associated.GetAllComponents(CType::Animation);
    for (auto& anim : anims) {
        ((Animation*)anim)->SoftPlay(id);
    }
}
