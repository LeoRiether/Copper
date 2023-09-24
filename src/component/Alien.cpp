#include "component/Alien.h"

#include <limits>

#include "CType.h"
#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Sound.h"
#include "component/Bullet.h"
#include "component/EndStateDimmer.h"
#include "component/KeepSoundAlive.h"
#include "component/Minion.h"
#include "component/Player.h"
#include "component/Sprite.h"
#include "state/EndState.h"
#include "util.h"

#define MODULE "Alien"

int Alien::alienCount = 0;

Alien::Alien(GameObject& go, int nMinions, float delay)
    : Component(go), minions(nMinions) {
    alienCount++;
    auto sprite = new Sprite{go, ASSETS "/img/alien.png"};
    associated.debugName = "Alien";
    go.AddComponent(sprite);
    go.AddComponent(new Collider{go});
    restTimer.Delay(delay);
}

Alien::~Alien() {
    // alienCount--; // done in Update
    minions.clear();
}

void Alien::Start() {
    auto& state = Game::Instance().GetState();

    for (size_t i = 0; i < minions.size(); i++) {
        auto minionGO = new GameObject{};
        auto sprite = new Sprite{*minionGO, ASSETS "/img/minion.png"};
        minionGO->AddComponent(sprite);
        float arc = 2 * PI * i / minions.size();
        auto minion = new Minion{*minionGO, state.GetObject(&associated), arc};
        minionGO->AddComponent(minion);
        minions[i] = state.RequestAddObject(minionGO);
    }
}

void Alien::Update(float dt) {
    switch (state) {
        case Resting: {
            if (restTimer.Get() >= REST_TIME_S) {
                state = Moving;
                if (Player::player) {
                    destination = Player::player->Associated().box.Center();
                }
            }
            break;
        }
        case Moving: {
            Vec2<Cart> delta = destination - associated.box.Center();
            if (delta.norm() <= SPEEEED * dt) {
                // Stop moving!
                state = Resting;
                restTimer.Restart();
                BulletHell();
            } else {
                speed = delta.normalize() * SPEEEED;
            }
            break;
        }
    }

    associated.box.x += speed.x * dt;
    associated.box.y += speed.y * dt;
    associated.angle -= dt * 0.3;

    if (hp <= 0) {
        associated.RequestDelete();
        alienCount--;

        // Create explosion sprite
        auto explosion = new GameObject{};
        auto sprite = new Sprite{*explosion, ASSETS "/img/aliendeath.png", 4,
                                 .15, 4 * .15};
        explosion->AddComponent(sprite);
        explosion->box.SetCenter(associated.box.Center());
        associated.RequestAdd(explosion);

        // Create explosion sound
        auto explosionSound = new GameObject{};
        auto sound = new Sound{*explosionSound, ASSETS "/audio/boom.wav"};
        auto keepalive = new KeepSoundAlive{*explosionSound};
        sound->Play();
        explosionSound->AddComponent(sound);
        explosionSound->AddComponent(keepalive);
        associated.RequestAdd(explosionSound);

        if (alienCount == 0) {
            // Create end stage dimmer
            auto dimmer = new GameObject{};
            dimmer->AddComponent(new EndStateDimmer{*dimmer, 2});
            associated.RequestAdd(dimmer);

            GameData::playerVictory =
                true;  // the aliens recognize the victory, how nice
        }
    }
}

void Alien::Render(Vec2<Cart>) {}

bool Alien::Is(CType type) { return type == CType::Alien; }

void Alien::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    if (!bullet || bullet->TargetsPlayer()) return;

    hp -= bullet->Damage();
}

Minion* Alien::ClosestMinion(Vec2<Cart> target) {
    float bestDist = std::numeric_limits<float>::max();
    Minion* bestMinion = nullptr;
    for (size_t i = 0; i < minions.size(); i++) {
        if (minions[i].expired()) continue;  // RIP

        Vec2<Cart> pos = minions[i].lock()->box.Center();
        if ((target - pos).norm2() < bestDist) {
            bestDist = (target - pos).norm2();
            bestMinion =
                (Minion*)minions[i].lock()->GetComponent(CType::Minion);
        }
    }

    return bestMinion;
}

void Alien::BulletHell() {
    auto player = Player::player;
    Vec2<Cart> target = player ? player->Associated().box.Center()
                               : Vec2<Cart>{(float)rng(), (float)rng()};
    auto minion = ClosestMinion(target);
    if (!minion) return;

    Vec2<Cart> direction = (target - associated.box.Center());

    const float delta = 7.0f * PI / 180.0f;
    for (int i = -3; i <= 3; i++) {
        Vec2<Cart> associatedTarget =
            direction.GetRotated(delta * i) + associated.box.Center();
        minion->Shoot(associatedTarget);
    }
}
