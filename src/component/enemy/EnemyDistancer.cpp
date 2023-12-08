#include "component/enemy/EnemyDistancer.h"

#include "Game.h"
#include "GameObject.h"
#include "Prefabs.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Player.h"
#include "component/enemy/RobotCan.h"
#include "math/Vec2.h"
#include "physics/CollisionEngine.h"
#include "physics/Steering.h"
#include "util.h"

#define MODULE "EnemyDistancer"

EnemyDistancer::EnemyDistancer(GameObject& go) : Component(go) {
    shotTimeout.Delay(randf(0, 1));
    roamingTimeout.Delay(randf(0, 1));
}

EnemyDistancer* EnemyDistancer::WithRobotCan(RobotCan* rc) {
    self = rc;
    return this;
}

void EnemyDistancer::Update(float dt) {
    if (!Player::player) return;

    auto roam = [&]() { associated.box.OffsetBy(roamingDelta * speed * dt); };

    auto walkAway = [&]() {
        auto playerPos = Player::player->Associated().box.Foot();
        auto enemyPos = associated.box.Center();
        auto distVec =
            (enemyPos - playerPos).normalize() +
            Steering{}.AddTerrain(enemyPos.toIso())->Result().toCart();
        distVec = distVec.normalize();
        self->direction = Direction::approxFromVec(distVec);

        const float stop2 = self->stopDistance * self->stopDistance;
        if (distVec.norm2() > stop2 &&
            (walkingTime >= 0.3 || walkingTime == 0)) {
            allAnimsPlay("idle_" + (-self->direction).toString());
            walkingTime = 0;
            return;
        }

        allAnimsPlay("walk_" + self->direction.toString());
        associated.box.OffsetBy(distVec * speed * dt);
        walkingTime += dt;
    };

    auto pursue = [&]() {
        auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
        if (!iso) fail("EnemyDistancer without IsoCollider...");
        auto selfPos = iso->box.Center().transmute<Iso>();
        auto maybePlayerPos = Player::player->LookForMe(iso->box);

        if (!maybePlayerPos) {
            switchState(Roaming);
            return;
        }

        auto moveDelta = (*maybePlayerPos - selfPos).toCart().normalize() +
                         Steering{}.AddTerrain(selfPos)->Result().toCart();
        moveDelta = moveDelta.normalize();

        self->direction = Direction::approxFromVec(moveDelta);
        allAnimsPlay("walk_" + self->direction.toString());

        auto realDistVec =
            Player::player->associated.box.Center() - selfPos.toCart();
        if (realDistVec.norm2() >= stopDistance * stopDistance) {
            self->associated.box.OffsetBy(moveDelta * speed * dt);
        } else {
            switchState(KeepingDistance);
        }
    };

    switch (state) {
        case WalkingAway: {
            walkAway();
            break;
        }
        case Roaming: {
            roam();

            roamingTimeout.Update(dt);
            if (roamingTimeout.Get() >= roamingTimeConst) {
                switchState(Standing);
            }

            if (seesPlayer()) switchState(Shooting);
            break;
        }
        case Standing: {
            standingStillTimeout.Update(dt);
            if (standingStillTimeout.Get() >= standingTimeConst) {
                switchState(Roaming);
            }

            if (seesPlayer()) switchState(Shooting);
            break;
        }
        case Shooting: {
            shotTimeout.Update(dt);
            if (shotTimeout.Get() >= shotAnimationTime) {
                switchState(Pursuing);
            }
            break;
        }
        case Pursuing: {
            pursue();

            pursueTimeout.Update(dt);
            if (pursueTimeout.Get() >= pursueTimeConst) {
                switchState(Shooting);
            }
            break;
        }
        case KeepingDistance: {
            roam();
            roamingTimeout.Update(dt);
            if (roamingTimeout.Get() >= roamingTimeConst) {
                switchState(Shooting);
            }
            break;
        }
    }
}

void EnemyDistancer::switchState(State newState) {
    auto shoot = [&]() {
        const auto playerPos = Player::player->Associated().box.Center();
        const auto delta = playerPos - associated.box.Center();
        auto go = MakeBullet(associated.box.Center(), delta.angle());
        Game::Instance().GetState().RequestAddObject(go);

        const auto dir = Direction::approxFromVec(delta);
        allAnimsPlay("fire1_" + dir.toString());

         auto decay = [&](float x) { return -x * x / 20'000.0f + 100.0f; };
         auto volume = decay(delta.norm());
         if (volume > 0) {
             associated.RequestAdd(
                 MakeOneOffAudio(ASSETS "/audio/Bullet_1.wav",
                 round(volume)));
         }
    };

    // Try to find an angle that doesn't immediately meet with a wall
    auto findRoamingDelta = [&]() {
        for (int tries = 0; tries < 10; tries++) {
            auto angle = randf(0, 2 * PI);
            roamingDelta = Vec2<Cart>{1, 0}.GetRotated(angle);

            auto center = associated.box.Center();
            auto forward = associated.box.Center() + roamingDelta * 80.0f;
            if (!CollisionEngine::TerrainContainsSegment(center.toIso(),
                                                         forward.toIso()))
                return;
        }
        warn("couldn't find roamingDelta");
    };

    // Get in new state
    switch (newState) {
        case Roaming: {
            findRoamingDelta();
            auto dir = Direction::approxFromVec(roamingDelta);
            allAnimsPlay("walk_" + dir.toString());

            roamingTimeout.Restart();
            roamingTimeout.Delay(randf(0, 0.5));
            break;
        }
        case Standing: {
            auto dir = Direction::approxFromVec(roamingDelta);
            allAnimsPlay("idle_" + dir.toString());
            standingStillTimeout.Restart();
            break;
        }
        case WalkingAway: {
            walkingTime = 0;
            break;
        }
        case Shooting: {
            shoot();
            shotTimeout.Restart();
            shotTimeout.Delay(randf(0, 0.5));
            break;
        }
        case Pursuing: {
            pursueTimeout.Restart();
            pursueTimeout.Delay(randf(0, 0.3));
            break;
        }
        case KeepingDistance: {
            findRoamingDelta();
            auto dir = Direction::approxFromVec(roamingDelta);
            allAnimsPlay("walk_" + dir.toString());

            roamingTimeout.Restart();
            roamingTimeout.Delay(randf(-0.6, 0.3));
            break;
        }
    }

    state = newState;
}

void EnemyDistancer::allAnimsPlay(const string& id) {
    auto& anims = associated.GetAllComponents(CType::Animation);
    for (auto& anim : anims) {
        ((Animation*)anim.get())->SoftPlay(id);
    }
}

bool EnemyDistancer::seesPlayer() {
    auto self = associated.box.Center().toIso();
    auto player = Player::player->associated.box.Center().toIso();
    return (player - self).norm() < 800 && !CollisionEngine::TerrainContainsSegment(self, player);
}
