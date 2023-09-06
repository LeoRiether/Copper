#include "Alien.h"

#include "InputManager.h"
#include "Sprite.h"
#include "util.h"

#define MODULE "Alien"

Alien::Alien(GameObject& go, int nMinions)
    : Component(go), go(go), minions(nMinions) {
    auto sprite = new Sprite{go, ASSETS "/img/alien.png"};
    go.AddComponent((Component*)sprite);
}

Alien::~Alien() {
    minions.clear();
    while (!tasks.empty()) tasks.pop();
}

void Alien::Start() {
    // TODO:
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
                Vec2 delta = task.pos - Vec2{go.box.x + go.box.w / 2.0f,
                                             go.box.y + go.box.h / 2.0f};
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

    go.box.x += speed.x * dt;
    go.box.y += speed.y * dt;

    if (hp <= 0) {
        go.RequestDelete();
    }
}

void Alien::Render(Vec2) {}

bool Alien::Is(CType type) { return type == CType::Alien; }
