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
#include "component/Collider.h"
#include "component/KillTimeout.h"
#include "component/Sound.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "math/Direction.h"
#include "physics/CollisionEngine.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "Player"

Player* Player::player;

Player::Player(GameObject& associated) : Component(associated) {
    Player::player = this;

    auto sprite = new Sprite{associated, ASSETS "/img/copper/spritesheet.png"};
    sprite->SetHasShadow(true);
    sprite->SetScale(150.0f / (sprite->SheetHeight() / 16.0f));
    associated.AddComponent(sprite);

    {
        auto anim = new Animation{associated, *sprite};
        GridKeyframe grid{20, 16, sprite->SheetWidth(), sprite->SheetHeight(),
                          0.02};

        auto row = [&](int i, int startJ, int frames, float frameTime) {
            Keyframes kf;
            for (int j = startJ; j < startJ + frames; j++) {
                grid.frameTime = frameTime;
                kf.push_back(grid.At(j, i));
            }
            return kf;
        };

        // This kind of row has a configurable progression of frameTimes
        // auto row2 = [&](int i, int startJ, vector<float> frameTimes) {
        //     Keyframes kf;
        //     int frames = frameTimes.size();
        //     for (int j = startJ; j < startJ + frames; j++) {
        //         grid.frameTime = frameTimes[j - startJ];
        //         kf.push_back(grid.At(j, i));
        //     }
        //     return kf;
        // };

        // This is a row modified to repeat the last frame a few times
        auto row3 = [&](int i, int startJ, int frames, float frameTime) {
            auto r = row(i, startJ, frames, frameTime);
            r.push_back(r.back());
            r.push_back(r.back());
            r.push_back(r.back());
            return r;
        };

        anim->AddKeyframes("S", row(0, 0, 20, 0.02));
        anim->AddKeyframes("SW", row(1, 0, 20, 0.02));
        anim->AddKeyframes("W", row(2, 0, 20, 0.02));
        anim->AddKeyframes("NW", row(3, 0, 20, 0.02));
        anim->AddKeyframes("N", row(4, 0, 20, 0.02));
        anim->AddKeyframes("NE", row(5, 0, 20, 0.02));
        anim->AddKeyframes("E", row(6, 0, 20, 0.02));
        anim->AddKeyframes("SE", row(7, 0, 20, 0.02));
        anim->AddKeyframes("attack_S", row3(8, 0, 20, 0.06));
        anim->AddKeyframes("attack_SW", row3(9, 0, 20, 0.06));
        anim->AddKeyframes("attack_W", row3(10, 0, 20, 0.06));
        anim->AddKeyframes("attack_NW", row3(11, 0, 20, 0.06));
        anim->AddKeyframes("attack_N", row3(12, 0, 20, 0.06));
        anim->AddKeyframes("attack_NE", row3(13, 0, 20, 0.06));
        anim->AddKeyframes("attack_E", row3(14, 0, 20, 0.06));
        anim->AddKeyframes("attack_SE", row3(15, 0, 20, 0.06));
        anim->AddKeyframes("idle_S", row(0, 3, 1, 1));
        anim->AddKeyframes("idle_SW", row(1, 3, 1, 1));
        anim->AddKeyframes("idle_W", row(2, 3, 1, 1));
        anim->AddKeyframes("idle_NW", row(3, 3, 1, 1));
        anim->AddKeyframes("idle_N", row(4, 3, 1, 1));
        anim->AddKeyframes("idle_NE", row(5, 3, 1, 1));
        anim->AddKeyframes("idle_E", row(6, 3, 1, 1));
        anim->AddKeyframes("idle_SE", row(7, 3, 1, 1));
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
        case Idle:
        case Walking:
        case Attacking:
        case StageTransition: {
            break;
        }
        case Dashing: {
            associated.angle = 0;
            dashState.timeout.Restart();
            break;
        }
    }

    auto anim = (Animation*)associated.GetComponent(CType::Animation);

    // Transition into new state
    state = newState;
    switch (newState) {
        case Idle: {
            anim->SoftPlay("idle_" + direction.toString());
            break;
        }
        case Walking: {
            stepsTimer.Restart();
            anim->SoftPlay(direction.toString());
            break;
        }
        case Dashing: {
            dashState = {};
            associated.angle = PI / 2.3;
            anim->SoftPlay("idle_" + direction.toString());
            break;
        }
        case Attacking: {
            attackState = {};

            auto& input = InputManager::Instance();
            direction = Direction::approxFromVec(input.Mouse() -
                                                 associated.box.Center());
            anim->SoftPlay("attack_" + direction.toString(), false);
            break;
        }
        case StageTransition: {
            stepsTimer.Restart();
            anim->SoftPlay(direction.toString());
            break;
        }
    }
}

void Player::MaybeChangeState(State newState) {
    if (state != newState) ChangeState(newState);
}

void Player::Update(float dt) {
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

    // Damage logic
    if (hpLoss > 0) {
        hpLossTimer.Update(dt);
        if (hpLossTimer.Get() > 1) {
            hpLossTimer.Restart();
            hp -= hpLoss <= hp ? hpLoss : hp;
            hpLoss = 0;
        }
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

    auto checkAttack = [&]() {
        if (input.MousePress(1)) {
            if (state == Attacking) {
                attackState.queuedAttack = true;
            } else if (state == Dashing) {
                dashState.queuedAttack = true;
            } else {
                ChangeState(Attacking);
            }
        }
    };

    auto deployCollider = [&]() {
        attackState.colliderDeployed = true;
        Rect hitbox{0, 0, 100, 100};
        hitbox.SetCenter(associated.box.Center() + direction.toVec() * 50.0f);
        auto go = new GameObject{};
        go->tags.set(tag::PlayerHitbox);
        go->tags.set(tag::Bullet); // a different kind of bullet...
        go->AddComponent(
            (new Collider{*go})->WithBase(hitbox));
        go->AddComponent(new KillTimeout{*go, 0.2});
        associated.RequestAdd(go);
    };

    switch (state) {
        case Idle: {
            auto currentDirection = Direction::fromInput();
            if (!currentDirection.isNone()) {
                direction = currentDirection;
                ChangeState(Walking);
            }
            checkDashEvent();
            checkAttack();
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
            checkAttack();
            makeStepSound();
            break;
        }
        case Dashing: {
            dashState.timeSinceStart.Update(dt);
            if (dashState.timeSinceStart.Get() >= DASH_DURATION) {
                ChangeState(dashState.queuedAttack ? Attacking : Idle);
            }
            checkAttack();
            break;
        }
        case Attacking: {
            checkAttack();

            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            if (!anim) fail("no associated Animation");
            int frame = anim->currentFrame;

            bool nextPhase = false;
            nextPhase |= attackState.phase == 0 && frame >= 11;
            nextPhase |= attackState.phase == 1 && frame >= 15;
            nextPhase |= attackState.phase == 2 && frame >= 22;
            if (nextPhase) {
                if (attackState.queuedAttack && attackState.phase < 2) {
                    attackState.phase++;
                    attackState.queuedAttack = false;
                    attackState.colliderDeployed = false;
                } else {
                    ChangeState(Idle);
                }
            }

            if (!attackState.colliderDeployed &&
                (frame == 5 || frame == 11 || frame == 19)) {
                deployCollider();
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
    auto getMoveVec = [&]() {
        moveVec = input.HasController() ? input.AxisVec(-1) : direction.toVec();
        return moveVec = moveVec * walkingSpeed;
    };

    switch (state) {
        case Idle: {
            moveVec = {0, 0};
            break;
        }
        case Walking:
        case StageTransition: {
            Vec2<Cart> speed = getMoveVec() * dt;
            associated.box.OffsetBy(speed);
            break;
        }
        case Dashing: {
            Vec2<Cart> speed = getMoveVec() * 2.5 * dt;
            associated.box.OffsetBy(speed);
            break;
        }
        case Attacking: {
            Vec2<Cart> speed = getMoveVec() * 0.2 * dt;
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
        // Takes damage
        hpLoss += bullet->Damage();
        hpLossTimer.Restart();

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
