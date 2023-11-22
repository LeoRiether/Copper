#include "component/enemy/EnemyFollower.h"

#include <bits/fs_fwd.h>

#include "component/Animation.h"
#include "component/Player.h"

void EnemyFollower::Update(Enemy& self, float dt) {
    if (!Player::player) return;

    auto allAnimsPlay = [&](const string& id) {
        auto anims = self.associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim)->SoftPlay(id);
        }
    };

    auto playerPos = Player::player->Associated().box.Foot();
    auto enemyPos = self.associated.box.Foot();
    auto distVec = playerPos - enemyPos;
    if (distVec.norm2() <= self.stopDistance * self.stopDistance) {
        allAnimsPlay("idle_" + self.direction.toString());
        return;
    }

    self.direction = Direction::approxFromVec(distVec);
    allAnimsPlay(self.direction.toString());
    self.associated.box.OffsetBy(distVec.normalize() * 300 * dt);
}
