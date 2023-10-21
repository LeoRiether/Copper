#include "component/enemy/EnemyFollower.h"

#include "component/Animation.h"
#include "component/Player.h"

void EnemyFollower::Update(Enemy &self, float dt) {
  if (!Player::player)
    return;

  auto anim = (Animation *)self.associated.GetComponent(CType::Animation);
  auto playerPos = Player::player->Associated().box.Foot();
  auto enemyPos = self.associated.box.Foot();
  auto distVec = playerPos - enemyPos;
  if (distVec.norm2() <= self.stopDistance * self.stopDistance) {
    anim->SoftPlay("idle_" + self.direction.toString());
    return;
  }

  self.direction = Direction::approxFromVec(distVec);
  anim->SoftPlay(self.direction.toString());
  self.associated.box.OffsetBy(distVec.normalize() * 300 * dt);
}
