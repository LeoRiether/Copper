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
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/EndStateDimmer.h"
#include "component/KeepSoundAlive.h"
#include "component/PenguinCannon.h"
#include "component/Sprite.h"
#include "component/TileMap.h"
#include "math/Direction.h"
#include "util.h"

#define MODULE "Player"

Player* Player::player;

Player::Player(GameObject& associated) : Component(associated) {
    Player::player = this;

    auto sprite = new Sprite{associated, ASSETS "/img/unicorn_atlas.png"};
    sprite->SetHasShadow(true);
    associated.AddComponent(sprite);

    {
        auto anim = new Animation{associated};
        GridKeyframe grid{24, 12, sprite->SheetWidth(), sprite->SheetHeight(),
                          0.1};

        auto row = [&](int i, int startJ, int frames) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++)
                kf.push_back(grid.At(j, i));
            return kf;
        };

        anim->AddKeyframes("E", row(0, 0, 6));
        anim->AddKeyframes("NE", row(0, 6, 6));
        anim->AddKeyframes("N", row(0, 12, 6));
        anim->AddKeyframes("NW", row(0, 18, 6));
        anim->AddKeyframes("W", row(1, 0, 6));
        anim->AddKeyframes("SW", row(1, 6, 6));
        anim->AddKeyframes("SE", row(1, 12, 6));
        anim->AddKeyframes("S", row(1, 18, 6));
        anim->AddKeyframes("idle_E", row(0, 0, 1));
        anim->AddKeyframes("idle_NE", row(0, 6, 1));
        anim->AddKeyframes("idle_N", row(0, 12, 1));
        anim->AddKeyframes("idle_NW", row(0, 18, 1));
        anim->AddKeyframes("idle_W", row(1, 0, 1));
        anim->AddKeyframes("idle_SW", row(1, 6, 1));
        anim->AddKeyframes("idle_SE", row(1, 12, 1));
        anim->AddKeyframes("idle_S", row(1, 18, 1));
        anim->Play("idle_S");  // just to kickstart the associated.box...
        associated.AddComponent(anim);
    }

    associated.AddComponent(new Collider{associated});
}

Player::~Player() { Player::player = nullptr; }

void Player::Start() { lastDirection = Direction{NoneX, Down}; }

void Player::Update(float dt) { UpdatePosition(dt); }

void Player::Render(Vec2<Cart> camera) {
    const auto& renderer = Game::Instance().Renderer();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 1400; i < 2000; i += 10) {
        for (int j = 100; j < 700; j += 10) {
            Vec2<Iso> iso{(float)i, (float)j};
            Vec2<Cart> cart = iso.toCart();
            SDL_Rect rect{(int)cart.x - (int)camera.x,
                          (int)cart.y - (int)camera.y, 3, 3};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

bool Player::Is(CType type) { return type == CType::Player; }

void Player::NotifyCollision(GameObject&) {}

void Player::RequestDelete() {
    associated.RequestDelete();
    Player::player = nullptr;
}

void Player::UpdatePosition(float dt) {
    auto direction = Direction::fromInput();
    auto anim = (Animation*)associated.GetComponent(CType::Animation);
    if (direction.x == NoneX && direction.y == NoneY) {
        anim->SoftPlay("idle_" + lastDirection.toString());
    } else {
        const float linearSpeed = 400;
        Vec2<Cart> speed = direction.toVec() * linearSpeed * dt;
        associated.box.x += speed.x;
        associated.box.y += speed.y;
        anim->SoftPlay(direction.toString());
        lastDirection = direction;
    }
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

    clamp(1400, iso.x, 2000);
    clamp(100, iso.y, 700);

    associated.box.SetFoot(iso.toCart());
}
