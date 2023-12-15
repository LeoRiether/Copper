#include "Powerups.h"

#include "GameObject.h"
#include "component/Animation.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "Powerups"

void Powerups::AddAnimatedSpriteTo(GameObject& go, int anim) {
    go.renderLayer = 1;
    auto sprite = new Sprite{go, ASSETS "/img/powerups.png"};

    auto animation = new Animation{go, *sprite};
    GridKeyframe grid{11, 9, sprite->SheetWidth(), sprite->SheetHeight(), 0.03};

    auto row = [&](int i) {
        Keyframes kf;
        for (int j = 0; j < grid.columns; j++) {
            kf.push_back(grid.At(j, i));
        }
        for (int j = grid.columns - 1; j >= 0; j--) {
            kf.push_back(kf[j]);
        }
        return kf;
    };

    animation->AddKeyframes("default", row(anim));
    animation->Play("default");

    go.AddComponent(sprite);
    go.AddComponent(animation);
}

float Powerups::ApplyToBulletTimeout(float timeout) {
    for (int i = next_bit(set, 0); i < 32; i = next_bit(set, i + 1))
        timeout = i == DoubledBullets ? timeout * 3.0f : timeout;
    return timeout;
}

float Powerups::ApplyToSpeed(float speed) {
    for (int i = next_bit(set, 0); i < 32; i = next_bit(set, i + 1))
        speed = i == Speedy ? speed * 1.3f : speed;
    return speed;
}

float Powerups::ApplyToAttackDamage(float damage) {
    for (int i = next_bit(set, 0); i < 32; i = next_bit(set, i + 1))
        damage = i == StrongerAttack ? damage * 1.3f : damage;
    return damage;
}
