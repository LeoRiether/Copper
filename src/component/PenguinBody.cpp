#include "component/PenguinBody.h"

#include <algorithm>
#include <cmath>

#include "Collider.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Sound.h"
#include "component/Bullet.h"
#include "component/EndStateDimmer.h"
#include "component/KeepSoundAlive.h"
#include "component/PenguinCannon.h"
#include "component/Sprite.h"
#include "component/TileMap.h"
#include "util.h"

#define MODULE "PenguinBody"

PenguinBody* PenguinBody::player;

PenguinBody::PenguinBody(GameObject& associated, weak_ptr<GameObject> tileMap)
    : Component(associated), tileMap(tileMap) {
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

    ReflectOnMapBorder();

    if (hp <= 0) {
        RequestDelete();
        GameData::playerVictory = false;

        // Create explosion sprite
        auto explosion = new GameObject{};
        auto sprite = new Sprite{*explosion, ASSETS "/img/penguindeath.png", 5,
                                 .15, 5 * .15};
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

        // Create end stage dimmer
        auto dimmer = new GameObject{};
        dimmer->AddComponent(new EndStateDimmer{*dimmer, 4});
        associated.RequestAdd(dimmer);
    }
}

void PenguinBody::Render(Vec2) {}

bool PenguinBody::Is(CType type) { return type == CType::PenguinBody; }

void PenguinBody::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    if (!bullet || !bullet->TargetsPlayer()) return;

    hp -= bullet->Damage();
}

void PenguinBody::RequestDelete() {
    associated.RequestDelete();
    PenguinBody::player = nullptr;
}

void PenguinBody::ReflectOnMapBorder() {
    if (tileMap.expired()) {
        warn("TileMap object not found?");
        return;
    }

    auto map = (TileMap*)tileMap.lock()->GetComponent(CType::TileMap);

    auto& box = associated.box;
    if (box.x < 0) {
        box.x = 0;
        angle = PI - angle;
    }
    if (box.x + box.w >= 1408) {
        box.x = 1408 - box.w;
        angle = PI - angle;
    }
    if (box.y < 0) {
        box.y = 0;
        angle = 2 * PI - angle;
    }
    if (box.y + box.h >= 1280) {
        box.y = 1280 - box.h;
        angle = 2 * PI - angle;
    }
}