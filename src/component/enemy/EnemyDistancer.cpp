#include "component/enemy/EnemyDistancer.h"

#include "component/Animation.h"
#include "component/Player.h"

void EnemyDistancer::Update(Enemy &self, float dt) {
  if (!Player::player)
    return;

  auto anim = (Animation *)self.associated.GetComponent(CType::Animation);
  auto playerPos = Player::player->Associated().box.Foot();
  auto enemyPos = self.associated.box.Foot();
  auto distVec = enemyPos - playerPos;
  self.direction = Direction::approxFromVec(distVec);

  const float stop2 = self.stopDistance * self.stopDistance;
  if (distVec.norm2() > stop2 && (walkingTime >= 0.3 || walkingTime == 0)) {
    anim->SoftPlay("idle_" + (-self.direction).toString());
    walkingTime = 0;
    return;
  }

  anim->SoftPlay(self.direction.toString());
  self.associated.box.OffsetBy(distVec.normalize() * 300 * dt);
  walkingTime += dt;
}
