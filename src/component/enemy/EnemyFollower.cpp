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

    auto allAnimsPlay = [&](const string& id, bool loops = true) {
        auto anims = associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim)->SoftPlay(id, loops);
        }
    };

    auto playerPos = Player::player->Associated().box.Foot();
    auto enemyPos = associated.box.Foot();
    auto distVec = playerPos - enemyPos;
    if (distVec.norm2() <= self->stopDistance * self->stopDistance) {
        allAnimsPlay("hide_" + self->direction.toString(), false);
        return;
    }

    self->direction = Direction::approxFromVec(distVec);
    allAnimsPlay("walk_" + self->direction.toString());
    self->associated.box.OffsetBy(distVec.normalize() * speed * dt);
}
