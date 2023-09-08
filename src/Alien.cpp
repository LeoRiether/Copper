#include "Alien.h"

#include <limits>

#include "CType.h"
#include "Game.h"
#include "InputManager.h"
#include "Minion.h"
#include "Sprite.h"
#include "util.h"

#define MODULE "Alien"

Alien::Alien(GameObject& go, int nMinions) : Component(go), minions(nMinions) {
    auto sprite = new Sprite{go, ASSETS "/img/alien.png"};
    associated.debugName = "Alien";
    go.AddComponent(sprite);
}

Alien::~Alien() {
    minions.clear();
    while (!tasks.empty()) tasks.pop();
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
    auto& input = InputManager::Instance();
    if (input.MousePress(LEFT_MOUSE_BUTTON))
        tasks.emplace(Action::Shoot, input.MouseX(), input.MouseY());
    if (input.MousePress(RIGHT_MOUSE_BUTTON))
        tasks.emplace(Action::Move, input.MouseX(), input.MouseY());

    if (!tasks.empty()) {
        auto task = tasks.front();
        switch (task.type) {
            case Action::Move: {
                Vec2 delta = task.pos - associated.box.Center();
                if (delta.norm() <= SPEEEED * dt) {
                    // Stop moving!
                    tasks.pop();
                    speed = Vec2{0, 0};
                } else {
                    speed = delta.normalize() * SPEEEED;
                }
                break;
            }
            case Action::Shoot: {
                Vec2 target{(float)input.MouseX(), (float)input.MouseY()};

                // Find minion closest to target
                float bestDist = std::numeric_limits<float>::max();
                Minion* bestMinion = nullptr;
                for (size_t i = 0; i < minions.size(); i++) {
                    Vec2 pos = minions[i].lock()->box.Center();
                    if ((target - pos).norm2() < bestDist) {
                        bestDist = (target - pos).norm2();
                        bestMinion = (Minion*)minions[i].lock()->GetComponent(
                            CType::Minion);
                    }
                }

                if (!bestMinion) {
                    warn("didn't find any minion to shoot?");
                } else {
                    bestMinion->Shoot(target);
                }
                tasks.pop();
                break;
            }
            default: {
                fail("unreachable");
                break;
            }
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
