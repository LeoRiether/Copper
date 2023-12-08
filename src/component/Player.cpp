#include "component/Player.h"

#include <cmath>
#include <string>

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Prefabs.h"
#include "SDL_render.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Sound.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "math/Direction.h"
#include "physics/CollisionEngine.h"
#include "util.h"

#define MODULE "Player"

Player* Player::player;

Player::Player(GameObject& associated) : Component(associated) {
    Player::player = this;

    auto sprite = new Sprite{associated, ASSETS "/img/copper_running.png"};
    sprite->SetHasShadow(true);
    sprite->SetScale(90.0f / (sprite->SheetHeight() / 8.0f));
    associated.AddComponent(sprite);

    {
        auto anim = new Animation{associated, *sprite};
        GridKeyframe grid{7, 8, sprite->SheetWidth(), sprite->SheetHeight(),
                          0.05};

        auto row = [&](int i, int startJ, int frames) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++)
                kf.push_back(grid.At(j, i));
            return kf;
        };

        anim->AddKeyframes("SE", row(0, 0, 7));
        anim->AddKeyframes("S", row(1, 0, 7));
        anim->AddKeyframes("SW", row(2, 0, 7));
        anim->AddKeyframes("W", row(3, 0, 7));
        anim->AddKeyframes("NW", row(4, 0, 7));
        anim->AddKeyframes("N", row(5, 0, 7));
        anim->AddKeyframes("NE", row(6, 0, 7));
        anim->AddKeyframes("E", row(7, 0, 7));
        anim->AddKeyframes("idle_SE", row(0, 0, 1));
        anim->AddKeyframes("idle_S", row(1, 0, 1));
        anim->AddKeyframes("idle_SW", row(2, 0, 1));
        anim->AddKeyframes("idle_W", row(3, 0, 1));
        anim->AddKeyframes("idle_NW", row(4, 0, 1));
        anim->AddKeyframes("idle_N", row(5, 0, 1));
        anim->AddKeyframes("idle_NE", row(6, 0, 1));
        anim->AddKeyframes("idle_E", row(7, 0, 1));
        anim->Play("idle_S");  // just to kickstart the associated.box...
        associated.AddComponent(anim);
    }
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
        case StageTransition: {
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
            stepsTimer.Restart();
            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            anim->SoftPlay(direction.toString());
            break;
        }
        case Dashing: {
            dashState.timeSinceStart.Restart();
            break;
        }
        case StageTransition: {
            stepsTimer.Restart();
            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            anim->SoftPlay(direction.toString());
            break;
        }
    }
}

void Player::MaybeChangeState(State newState) {
    if (state != newState) ChangeState(newState);
}

void Player::Update(float dt) {
	if (!Game::Instance().GetState().playerMovement)
		return;
    UpdateState(dt);
    UpdatePosition(dt);

    // Flash reset
    flashTimeout -= dt;
    if (flashTimeout <= 0) {
        flashTimeout = INFINITY;  // won't trigger this part again very soon
        auto sprite = (Sprite*)associated.GetComponent(CType::Sprite);
        sprite->WithFlash(false);
    }

    // Leave trail
    trailTimer.Update(dt);
    if (trailTimer.Get() >= 0.1) {
        trailTimer.Restart();
        if (Trail.size() >= 60) Trail.erase(Trail.begin());

        auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
        if (!iso) fail("player without IsoCollider...");
        Trail.push_back(iso->box.Center().transmute<Iso>());
    }
}

void Player::UpdateState(float dt) {
    auto& input = InputManager::Instance();

    auto checkDashEvent = [&]() {
        dashState.timeout.Update(dt);
        if (dashState.timeout.Get() >= DASH_TIMEOUT &&
            (input.KeyPress(DASH_KEY) ||
             input.ControllerPress(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))) {
            ChangeState(Dashing);
        }
    };

    auto makeStepSound = [&]() {
        return;
        stepsTimer.Update(dt);
        if (stepsTimer.Get() >= stepsTiming) {
            stepsTimer.Restart();
            stepsTimer.Delay(0.1);

            auto sound = (Sound*)associated.GetComponent(CType::Sound);
            if (!sound) {
                warn("no associated Sound?");
                return;
            }
            sound->Play();
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
            makeStepSound();
            break;
        }
        case Dashing: {
            dashState.timeSinceStart.Update(dt);
            if (dashState.timeSinceStart.Get() >= DASH_DURATION) {
                ChangeState(Idle);
            }
            break;
        }
        case StageTransition: {
            makeStepSound();
            break;
        }
    }
}

void Player::UpdatePosition(float dt) {
    associated.box.OffsetBy(knockbackVelocity * dt);
    knockbackVelocity = knockbackVelocity * 0.70;

    auto& input = InputManager::Instance();
    auto moveVec = [&]() {
        return input.HasController() ? input.AxisVec(-1) : direction.toVec();
    };

    switch (state) {
        case Idle: {
            break;
        }
        case Walking:
        case StageTransition: {
            Vec2<Cart> speed = moveVec() * walkingSpeed * dt;
            associated.box.OffsetBy(speed);
            break;
        }
        case Dashing: {
            Vec2<Cart> speed = moveVec() * walkingSpeed * 2.5 * dt;
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
    auto drawAnIsometricSquareOnTheGround = [&]() {
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
    };

    auto drawPlayerPosition = [&]() {
        static GameObject* text;
        if (text == nullptr) {
            text = new GameObject{};
            text->AddComponent(new Text{*text, ASSETS "/font/Call me maybe.ttf",
                                        30, Text::Blended, "?",
                                        SDL_Color{255, 255, 0, 255}});
        }
        auto textComponent = (Text*)text->GetComponent(CType::Text);
        auto pos = associated.box.Foot().toCart();
        textComponent->SetText(std::to_string(pos.x) + ", " +
                               std::to_string(pos.y));
        text->box.SetFoot({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 10});
        textComponent->Render(Vec2<Cart>{0, 0});
    };

    auto drawChunk = [&]() {
        auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
        if (!iso) fail("no IsoCollider");
        auto pos = iso->box.Center().transmute<Iso>().toCart();
        int i = int(floor(pos.x)) / 256 * 256;
        int j = int(floor(pos.y)) / 256 * 256;
        SDL_FRect rect{i - camera.x, j - camera.y, 256, 256};
        auto renderer = Game::Instance().Renderer();
        SDL_SetRenderDrawColor(renderer, 92, 134, 178, 100);
        SDL_RenderFillRectF(renderer, &rect);
    };
    static int& showChunk = Consts::GetInt("debug.show_chunk");
    if (showChunk) drawChunk();

    // Draw trail
    static int& showTrail = Consts::GetInt("debug.show_trail");
    if (showTrail && !Trail.empty()) {
        SDL_FRect rects[Trail.size()];
        for (int i = 0; i < (int)Trail.size(); i++) {
            auto pos = Trail[i].toCart() - camera;
            rects[i] = {pos.x - 5, pos.y - 5, 10, 10};
        }
        auto renderer = Game::Instance().Renderer();
        SDL_SetRenderDrawColor(renderer, 209, 32, 229, 128);
        SDL_RenderFillRectsF(renderer, rects, Trail.size());
    }
}

void Player::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);
    if (bullet && bullet->TargetsPlayer()) {
        // Flash
        auto sprite = (Sprite*)associated.GetComponent(CType::Sprite);
        sprite->WithFlash(true);
        flashTimeout = 0.03;

        // Explosion
        auto hitpoint = other.box.Center();
        hitpoint = hitpoint + Vec2<Cart>{25, 0}.GetRotated(other.angle);
        associated.RequestAdd(MakeExplosion1()->WithCenterAt(hitpoint));

        // Knockback
        float kb = 1'500'000 * Game::Instance().DeltaTime();
        knockbackVelocity = Vec2<Cart>{kb, 0}.GetRotated(other.angle);

        // Slowdown
        Game::Instance().Slowdown(0.03, 0.1);

        // Add trauma
        Game::Instance().AddTrauma(0.4);

        other.RequestDelete();
    }
}

void Player::RequestDelete() {
    associated.RequestDelete();
    Player::player = nullptr;
}

std::optional<Vec2<Iso>> Player::LookForMe(Rect iv) {
    auto ok = [&](Vec2<Iso> me) {
        auto c = [&](Vec2<Cart> p) {
            auto pi = p.transmute<Iso>();
            return !CollisionEngine::TerrainContainsSegment(pi, me);
        };
        return c(iv.Center());
        // return c(iv.TopLeft()) && c(iv.TopRight()) && c(iv.BotLeft()) &&
        //        c(iv.BotRight());
    };

    auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
    if (!iso) fail("no associated IsoCollider");
    if (ok(iso->box.Center().transmute<Iso>())) {
        return iso->box.Center().transmute<Iso>();
    }

    for (int i = (int)Trail.size() - 1; i >= 0; i--) {
        if (ok(Trail[i])) return Trail[i];
    }

    return {};
}
