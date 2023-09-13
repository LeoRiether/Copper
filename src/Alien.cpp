#include "Alien.h"

#include <limits>

#include "Bullet.h"
#include "CType.h"
#include "Collider.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Minion.h"
#include "PenguinBody.h"
#include "Sprite.h"
#include "util.h"

#define MODULE "Alien"

int Alien::alienCount = 0;

Alien::Alien(GameObject& go, int nMinions) : Component(go), minions(nMinions) {
    alienCount++;
    auto sprite = new Sprite{go, ASSETS "/img/alien.png"};
    associated.debugName = "Alien";
    go.AddComponent(sprite);
    go.AddComponent(new Collider{go});
}

Alien::~Alien() {
    alienCount--;
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
            restTimer.Update(dt);
            if (restTimer.Get() >= REST_TIME_S) {
                restTimer.Restart();
                state = Moving;
                if (PenguinBody::player) {
                    destination =
                        PenguinBody::player->Associated().box.Center();
                }
            }
            break;
        }
        case Moving: {
            Vec2 delta = destination - associated.box.Center();
            if (delta.norm() <= SPEEEED * dt) {
                // Stop moving!
                state = Resting;
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
    }
}

void Alien::Render(Vec2) {}

bool Alien::Is(CType type) { return type == CType::Alien; }

Minion* Alien::ClosestMinion(Vec2 target) {
    float bestDist = std::numeric_limits<float>::max();
    Minion* bestMinion = nullptr;
    for (size_t i = 0; i < minions.size(); i++) {
        Vec2 pos = minions[i].lock()->box.Center();
        if ((target - pos).norm2() < bestDist) {
            bestDist = (target - pos).norm2();
            bestMinion =
                (Minion*)minions[i].lock()->GetComponent(CType::Minion);
        }
    }

    if (!bestMinion) {
        warn("didn't find any closest minion?");
    }

    return bestMinion;
}

void Alien::BulletHell() {
    auto player = PenguinBody::player;
    Vec2 target = player ? player->Associated().box.Center()
                         : Vec2{(float)rng(), (float)rng()};
    auto minion = ClosestMinion(target);

    Vec2 direction = (target - associated.box.Center());

    const float delta = 7.0f * PI / 180.0f;
    for (int i = -3; i <= 3; i++) {
        Vec2 associatedTarget =
            direction.GetRotated(delta * i) + associated.box.Center();
        minion->Shoot(associatedTarget);
    }
}
