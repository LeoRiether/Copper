#include "Alien.h"

#include "Game.h"
#include "InputManager.h"
#include "Minion.h"
#include "Sprite.h"
#include "util.h"

#define MODULE "Alien"

Alien::Alien(GameObject& go, int nMinions) : Component(go), minions(nMinions) {
    auto sprite = new Sprite{go, ASSETS "/img/alien.png"};
    go.AddComponent((Component*)sprite);
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
        float arc = 2 * PI * i / minions.size();
        auto minion = new Minion{*minionGO, state.GetObject(&associated), arc};
        minionGO->AddComponent((Component*)sprite);
        minionGO->AddComponent((Component*)minion);
        minions[i] = state.AddObject(minionGO);
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
                Vec2 delta =
                    task.pos - Vec2{associated.box.x + associated.box.w / 2.0f,
                                    associated.box.y + associated.box.h / 2.0f};
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
                // TODO:
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

    if (hp <= 0) {
        associated.RequestDelete();
    }
}

void Alien::Render(Vec2) {}

bool Alien::Is(CType type) { return type == CType::Alien; }
