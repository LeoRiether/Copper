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

void Player::Start() {
    direction = Direction{NoneX, Down};
    ChangeState(Idle);
}

// TODO: maybe only transition on the next update?
void Player::ChangeState(State newState) {
    // Transition out of old state
    switch (state) {
        case Idle: {
            break;
        }
        case Walking: {
            break;
        }
        case Dashing: {
            dashState.timeout.Restart();
            break;
        }
    }

    // Transition into new state
    state = newState;
    switch (newState) {
        case Idle: {
            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            anim->SoftPlay("idle_" + direction.toString());
            break;
        }
        case Walking: {
            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            anim->SoftPlay(direction.toString());
            break;
        }
        case Dashing: {
            dashState.timeSinceStart.Restart();
            break;
        }
    }
}

void Player::MaybeChangeState(State newState) {
    if (state != newState) ChangeState(newState);
}

void Player::Update(float dt) {
    UpdateState();
    UpdatePosition(dt);
}

void Player::UpdateState() {
    auto& input = InputManager::Instance();

    auto checkDashEvent = [&]() {
        if (dashState.timeout.Get() >= DASH_TIMEOUT &&
            input.KeyPress(DASH_KEY)) {
            ChangeState(Dashing);
        }
    };

    switch (state) {
        case Idle: {
            auto currentDirection = Direction::fromInput();
            if (!currentDirection.isNone()) {
                direction = currentDirection;
                ChangeState(Walking);
            }
            checkDashEvent();
            break;
        }
        case Walking: {
            auto currentDirection = Direction::fromInput();
            if (currentDirection.isNone()) {
                ChangeState(Idle);
            } else if (direction != currentDirection) {
                // Force the right animation to play.
                // Not sure if this is the best way to do it
                direction = currentDirection;
                ChangeState(Walking);
            } else {
                direction = currentDirection;
            }
            checkDashEvent();
            break;
        }
        case Dashing: {
            if (dashState.timeSinceStart.Get() >= DASH_DURATION) {
                ChangeState(Idle);
            }
            break;
        }
    }
}

void Player::UpdatePosition(float dt) {
    switch (state) {
        case Idle: {
            break;
        }
        case Walking: {
            const float linearSpeed = 400;
            Vec2<Cart> speed = direction.toVec() * linearSpeed * dt;
            associated.box.OffsetBy(speed);
            break;
        }
        case Dashing: {
            const float linearSpeed = 800;
            Vec2<Cart> speed = direction.toVec() * linearSpeed * dt;
            associated.box.OffsetBy(speed);
            break;
        }
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
