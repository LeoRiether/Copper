#include "component/powerup/StrongerAttack.h"

#include "Powerups.h"
#include "component/Player.h"

#define MODULE "StrongerAttack"

StrongerAttack::StrongerAttack(GameObject& go) : Component(go) {
    Powerups::AddAnimatedSpriteTo(go, 2);
}

void StrongerAttack::Update(float dt) {
    if (!started && !associated.GetComponent(CType::Pickupable)) start();
    if /* still hasn't */ (!started) return;

    auto player = Player::player;
    if (player) {
        associated.box.SetFoot(player->associated.box.Head() +
                               Vec2<Cart>{0, 30});
    }

    timeout.Update(dt);
    if (timeout.Get() >= timeAlive) {
        associated.RequestDelete();
        if (player) player->RemovePowerup(Powerups::StrongerAttack);
    }
}

void StrongerAttack::start() {
    if (Player::player) Player::player->AddPowerup(Powerups::StrongerAttack);
    started = true;
}
