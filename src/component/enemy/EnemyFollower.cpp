#include "component/enemy/EnemyFollower.h"

#include "component/Animation.h"
#include "component/Player.h"

EnemyFollower::EnemyFollower(GameObject& go) : Component(go) {}

EnemyFollower* EnemyFollower::WithRobotCan(RobotCan* rc) {
    self = rc;
    return this;
}

void EnemyFollower::Update(float dt) {
    if (!Player::player) return;

    auto allAnimsPlay = [&](const string& id) {
        auto anims = associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim)->SoftPlay(id);
        }
    };

    auto playerPos = Player::player->Associated().box.Foot();
    auto enemyPos = associated.box.Foot();
    auto distVec = playerPos - enemyPos;
    if (distVec.norm2() <= self->stopDistance * self->stopDistance) {
        allAnimsPlay("idle_" + self->direction.toString());
        return;
    }

    self->direction = Direction::approxFromVec(distVec);
    allAnimsPlay(self->direction.toString());
    self->associated.box.OffsetBy(distVec.normalize() * 300 * dt);
}
