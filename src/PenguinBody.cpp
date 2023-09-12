#include "PenguinBody.h"

#include <algorithm>
#include <cmath>

#include "Bullet.h"
#include "Collider.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "KeepSoundAlive.h"
#include "PenguinCannon.h"
#include "Sound.h"
#include "Sprite.h"
#include "util.h"

#define MODULE "PenguinBody"

PenguinBody* PenguinBody::player;

PenguinBody::PenguinBody(GameObject& associated) : Component(associated) {
    PenguinBody::player = this;

    auto sprite = new Sprite{associated, ASSETS "/img/penguin.png"};
    associated.AddComponent(sprite);
    associated.AddComponent(new Collider{associated});
}

PenguinBody::~PenguinBody() { PenguinBody::player = nullptr; }

void PenguinBody::Start() {
    auto& state = Game::Instance().GetState();
    auto go = new GameObject{};
    auto cannon = new PenguinCannon{*go, state.GetObject(&associated)};
    go->AddComponent(cannon);
    associated.RequestAdd(go);
}

void PenguinBody::Update(float dt) {
    auto& input = InputManager::Instance();
    if (input.IsKeyDown(MOVE_FORWARD_KEY)) {
        linearSpeed = std::min(linearSpeed + 1000.0f * dt, +400.0f);
    }
    if (input.IsKeyDown(MOVE_BACKWARDS_KEY)) {
        linearSpeed = std::max(linearSpeed - 1000.0f * dt, -400.0f);
    }

    float sign = linearSpeed >= 0 ? 1 : -1;
    if (input.IsKeyDown(ROTATE_CLOCKWISE_KEY)) {
        angle += 3 * dt * sign;
    }
    if (input.IsKeyDown(ROTATE_COUNTERCLOCKWISE_KEY)) {
        angle -= 3 * dt * sign;
    }

    speed = Vec2{linearSpeed, 0}.GetRotated(angle);

    associated.box.x += speed.x * dt;
    associated.box.y += speed.y * dt;

    associated.angle = angle;

    if (hp <= 0) {
        associated.RequestDelete();

        // Create explosion sprite
        auto explosion = new GameObject{}; 
        auto sprite = new Sprite{*explosion, ASSETS "/img/penguindeath.png", 5, .15, 5 * .15};
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
    }
}

void PenguinBody::Render(Vec2) {}

bool PenguinBody::Is(CType type) { return type == CType::PenguinBody; }

void PenguinBody::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    if (!bullet || !bullet->TargetsPlayer()) return;

    hp -= bullet->Damage();
}
