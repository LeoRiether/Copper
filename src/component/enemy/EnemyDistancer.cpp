#include "component/enemy/EnemyDistancer.h"

#include "Game.h"
#include "GameObject.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Player.h"
#include "component/enemy/RobotCan.h"
#include "math/Vec2.h"
#include "physics/CollisionEngine.h"

#define MODULE "EnemyDistancer"

EnemyDistancer::EnemyDistancer(GameObject& go) : Component(go) {}

EnemyDistancer* EnemyDistancer::WithRobotCan(RobotCan* rc) {
    self = rc;
    return this;
}

void EnemyDistancer::Update(float dt) {
    if (!Player::player) return;

    auto roam = [&]() { associated.box.OffsetBy(roamingDelta * speed * dt); };

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

        allAnimsPlay("walk_" + self->direction.toString());
        associated.box.OffsetBy(distVec.normalize() * speed * dt);
        walkingTime += dt;
    };

    switch (state) {
        case Roaming: {
            roam();

            roamingTimeout.Update(dt);
            if (roamingTimeout.Get() >= roamingTimeConst) {
                switchState(Shooting);
            }

            break;
        }
        case WalkingAway: {
            walkAway();

            break;
        }
        case Shooting: {
            shotTimeout.Update(dt);
            if (shotTimeout.Get() >= shotAnimationTime) {
                switchState(Roaming);
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
        case Roaming: {
            break;
        }
        case Shooting: {
            break;
        }
    }

    auto shoot = [&]() {
        const auto playerPos = Player::player->Associated().box.Foot();
        const auto delta = playerPos - associated.box.Center();
        auto go = Bullet::Make(associated.box.Center(), delta.angle());
        Game::Instance().GetState().RequestAddObject(go);

        // TODO: shoot animation
        const auto dir = Direction::approxFromVec(delta);
        allAnimsPlay("fire1_" + dir.toString());
    };

    // Try to find an angle that doesn't immediately meet with a wall
    auto findRoamingDelta = [&]() {
        for (int tries = 0; tries < 10; tries++) {
            auto angle = randf(0, 2 * PI);
            roamingDelta = Vec2<Cart>{1, 0}.GetRotated(angle);

            auto aFrameForward =
                associated.box.Center() + roamingDelta * speed / 16.0;
            if (!CollisionEngine::TerrainContains(aFrameForward.toIso()))
                return;
        }
        warn("couldn't find roamingDelta");
    };

    // Get in new state
    switch (newState) {
        case WalkingAway: {
            walkingTime = 0;
            break;
        }
        case Roaming: {
            findRoamingDelta();
            auto dir = Direction::approxFromVec(roamingDelta);
            allAnimsPlay("walk_" + dir.toString());

            roamingTimeout.Restart();
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
