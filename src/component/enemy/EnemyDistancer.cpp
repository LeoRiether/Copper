#include "component/enemy/EnemyDistancer.h"

#include "component/Animation.h"
#include "component/Player.h"

void EnemyDistancer::Update(RobotCan& self, float dt) {
    if (!Player::player) return;

    auto allAnimsPlay = [&](const string& id) {
        auto anims = self.associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim)->SoftPlay(id);
        }
    };

    auto playerPos = Player::player->Associated().box.Foot();
    auto enemyPos = self.associated.box.Foot();
    auto distVec = enemyPos - playerPos;
    self.direction = Direction::approxFromVec(distVec);

    const float stop2 = self.stopDistance * self.stopDistance;
    if (distVec.norm2() > stop2 && (walkingTime >= 0.3 || walkingTime == 0)) {
        allAnimsPlay("idle_" + (-self.direction).toString());
        walkingTime = 0;
        return;
    }

    allAnimsPlay(self.direction.toString());
    self.associated.box.OffsetBy(distVec.normalize() * 300 * dt);
    walkingTime += dt;
}
