#include "component/Player.h"

#include <SDL2/SDL_render.h>

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

#define MODULE "Player"

Player* Player::player;

Player::Player(GameObject& associated, weak_ptr<GameObject> tileMap)
    : Component(associated), tileMap(tileMap) {
    Player::player = this;

    auto sprite =
        new Sprite{associated, ASSETS "/img/elements_east_walk.png", 6, 0.1};
    sprite->SetScale(Vec2<Cart>{1, 1});
    associated.AddComponent(sprite);
    associated.AddComponent(new Collider{associated});
}

Player::~Player() { Player::player = nullptr; }

void Player::Start() {}

void Player::Update(float dt) {
    auto& input = InputManager::Instance();

    Vec2<Cart> speed{0, 0};
    if (input.IsKeyDown(MOVE_UP_KEY)) {
        speed.y -= 300 * dt;
    } else if (input.IsKeyDown(MOVE_DOWN_KEY)) {
        speed.y += 300 * dt;
    }
    if (input.IsKeyDown(MOVE_RIGHT_KEY)) {
        speed.x += 600 * dt;
    } else if (input.IsKeyDown(MOVE_LEFT_KEY)) {
        speed.x -= 600 * dt;
    }

    if (speed.x != 0 && speed.y != 0) speed = speed / sqrt(2);

    associated.box.x += speed.x;
    associated.box.y += speed.y;

    ConstrainToTile();

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

void Player::Render(Vec2<Cart> camera) {
    const auto& renderer = Game::Instance().Renderer();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 1400; i < 1800; i += 10) {
        for (int j = 100; j < 500; j += 10) {
            Vec2<Iso> iso{(float)i, (float)j};
            Vec2<Cart> cart = iso.toCart();
            SDL_Rect rect{(int)cart.x - (int)camera.x,
                          (int)cart.y - (int)camera.y, 3, 3};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

bool Player::Is(CType type) { return type == CType::Player; }

void Player::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    if (!bullet || !bullet->TargetsPlayer()) return;

    hp -= bullet->Damage();
}

void Player::RequestDelete() {
    associated.RequestDelete();
    Player::player = nullptr;
}

void Player::ConstrainToTile() {
    return;
    Vec2<Iso> iso = associated.box.Foot().toIso();

    auto clamp = [&](float mn, float& x, float mx) {
        if (x < mn)
            x = mn;
        else if (x > mx)
            x = mx;
    };

    clamp(1400, iso.x, 1800);
    clamp(100, iso.y, 500);

    associated.box.SetFoot(iso.toCart());
}
