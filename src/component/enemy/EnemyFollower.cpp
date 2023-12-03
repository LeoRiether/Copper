#include "component/enemy/EnemyFollower.h"

#include "component/Animation.h"
#include "component/IsoCollider.h"
#include "component/Player.h"
#include "util.h"

#define MODULE "EnemyFollower"

EnemyFollower::EnemyFollower(GameObject& go) : Component(go) {}

EnemyFollower* EnemyFollower::WithRobotCan(RobotCan* rc) {
    self = rc;
    return this;
}

void EnemyFollower::Update(float dt) {
    if (!Player::player) return;

    auto allAnimsPlay = [&](const string& id, bool loops = true) {
        auto& anims = associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim.get())->SoftPlay(id, loops);
        }
    };

    auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
    if (!iso) fail("companion without IsoCollider...");
    auto selfPos = iso->box.Center().transmute<Iso>();
    auto maybePlayerPos = Player::player->LookForMe(iso->box);

    if (!maybePlayerPos) {
        allAnimsPlay("hide_" + self->direction.toString(), false);
        return;
    }

    auto distVec = (*maybePlayerPos - selfPos).toCart();
    self->direction = Direction::approxFromVec(distVec);
    allAnimsPlay("walk_" + self->direction.toString());

    auto actualPlayerDistVec =
        Player::player->associated.box.Center() - selfPos.toCart();
    if (actualPlayerDistVec.norm2() >=
        self->stopDistance * self->stopDistance) {
        self->associated.box.OffsetBy(distVec.normalize() * speed * dt);
    }
}
