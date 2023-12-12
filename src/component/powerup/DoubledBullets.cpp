#include "component/powerup/DoubledBullets.h"

#include "Powerups.h"
#include "component/Player.h"

#define MODULE "DoubledBullets"

DoubledBullets::DoubledBullets(GameObject& go) : Component(go) {
    Powerups::AddAnimatedSpriteTo(go, 5);
}

void DoubledBullets::Update(float dt) {
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
        if (player) player->RemovePowerup(Powerups::DoubledBullets);
    }
}

void DoubledBullets::start() {
    if (Player::player) Player::player->AddPowerup(Powerups::DoubledBullets);
    started = true;
}
