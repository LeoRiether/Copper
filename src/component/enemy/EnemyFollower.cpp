#include "component/enemy/EnemyFollower.h"

#include "Consts.h"
#include "Game.h"
#include "Prefabs.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Collider.h"
#include "component/IsoCollider.h"
#include "component/KillTimeout.h"
#include "component/OverheadHpBar.h"
#include "component/Player.h"
#include "physics/Steering.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "EnemyFollower"

EnemyFollower::EnemyFollower(GameObject& go) : Component(go) {
    auto sprite = new Sprite{associated, ASSETS "/img/copper/enemy.png"};
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

        // This is a row modified to repeat the last frame a few times
        auto row3 = [&](int i, int startJ, int frames, float frameTime) {
            auto r = row(i, startJ, frames, frameTime);
            r.push_back(r.back());
            r.push_back(r.back());
            return r;
        };

        anim->AddKeyframes("walk_S", row(0, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_SW", row(1, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_W", row(2, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_NW", row(3, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_N", row(4, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_NE", row(5, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_E", row(6, 0, 20, 0.02 * 1.5f));
        anim->AddKeyframes("walk_SE", row(7, 0, 20, 0.02 * 1.5f));
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

void EnemyFollower::Start() {
    Game::Instance().GetState().GetCamera().SecondaryFollow(associated.weak);
    spawnPoint = associated.box.Center();
}

//////////////////////////
//        Update        //
//////////////////////////
void EnemyFollower::Update(float dt) {
    if (!Player::player) return;

    knockbackVelocity = knockbackVelocity * 0.70;
    stunnedLevel = std::max<float>(0.0, stunnedLevel - dt);

    updateState(dt);
    updatePosition(dt);
}

void EnemyFollower::updateState(float) {
    auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
    if (!iso) fail("EnemyFollower without IsoCollider...");
    auto selfPos = iso->box.Center().transmute<Iso>();

    switch (state) {
        case Roaming: {
            auto maybePlayerPos = Player::player->LookForMe(iso->box);
            if (maybePlayerPos) {
                switchState(Pursuing);
                break;
            }

            if ((roamingTarget - selfPos.toCart()).norm2() <= 1) {
                switchState(Roaming);
                break;
            }
            break;
        }
        case Pursuing: {
            break;
        }
        case Attacking: {
            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            if (!anim) fail("no associated Animation");

            if (anim->currentFrame >= 21) {
                switchState(Pursuing);
            }
            break;
        }
    }
}

void EnemyFollower::updatePosition(float dt) {
    auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
    if (!iso) fail("EnemyFollower without IsoCollider...");
    auto selfPos = iso->box.Center().transmute<Iso>();

    auto moveBy = [&](Vec2<Cart> delta) {
        delta = delta * std::max(0.05f, 1 - stunnedLevel);
        associated.box.OffsetBy(delta);
    };

    associated.box.OffsetBy(knockbackVelocity * dt);

    auto deployCollider = [&]() {
        attackState.phase++;

        Rect hitbox{0, 0, 100, 100};
        hitbox.SetCenter(associated.box.Center() + direction.toVec() * 50.0f);

        // Collider
        auto go = new GameObject{};
        go->tags.set(tag::EnemyHitbox);
        go->tags.set(tag::Bullet);  // a different kind of bullet...
        go->AddComponent((new Collider{*go})->WithBase(hitbox));
        go->AddComponent(new KillTimeout{*go, 0.2});
        associated.RequestAdd(go);

        // Slash sprite
        auto pos = (associated.box.Center() + direction.toVec() * 60.0f);
        auto slash = MakeSlash(pos, direction.toVec().angle());
        slash->angle = direction.toVec().angle();
        associated.RequestAdd(slash);
    };

    switch (state) {
        case Roaming: {
            moveDelta = (roamingTarget - selfPos.toCart()).normalize() +
                        Steering{}.AddTerrain(selfPos)->Result().toCart();
            moveDelta = moveDelta.normalize();
            moveBy(moveDelta * speed * dt);
            break;
        }
        case Pursuing: {
            auto maybePlayerPos = Player::player->LookForMe(iso->box);
            if (!maybePlayerPos) {
                switchState(Roaming);
                break;
            }

            moveDelta = (*maybePlayerPos - selfPos).toCart().normalize() +
                        Steering{}.AddTerrain(selfPos)->Result().toCart();
            moveDelta = moveDelta.normalize();

            direction = Direction::approxFromVec(moveDelta);
            animsSoftPlay("walk_" + direction.toString());
            auto realDistVec =
                Player::player->associated.box.Center() - selfPos.toCart();
            if (realDistVec.norm2() >= attackDistance * attackDistance) {
                moveBy(moveDelta * speed * dt);
            } else if (stunnedLevel < 0.1) {
                switchState(Attacking);
            }
            break;
        }
        case Attacking: {
            auto anim = (Animation*)associated.GetComponent(CType::Animation);
            if (!anim) fail("no associated Animation");
            int frame = anim->currentFrame;

            bool nextPhase = false;
            nextPhase |= attackState.phase == 0 && frame >= 5;
            nextPhase |= attackState.phase == 1 && frame >= 11;
            nextPhase |= attackState.phase == 2 && frame >= 19;
            if (nextPhase) deployCollider();

            break;
        }
    }
}

void EnemyFollower::switchState(State newState) {
    switch (newState) {
        case Roaming: {
            if ((associated.box.Center() - spawnPoint).norm2() >= 700 * 700) {
                // Too far, choose new spawn point ¯\_(ツ)_/¯
                spawnPoint = associated.box.Center();
            }

            auto displacement =
                Vec2<Cart>{sqrt(randf(0, 400)), 0}.GetRotated(randf(0, 2 * PI));
            roamingTarget = spawnPoint + displacement;
            direction = Direction::approxFromVec(roamingTarget -
                                                 associated.box.Center());
            animsSoftPlay("walk_" + direction.toString());
            break;
        }
        case Pursuing: {
            animsSoftPlay("walk_" + direction.toString());
            break;
        }
        case Attacking: {
            auto delta = Player::player->associated.box.Center() -
                         associated.box.Center();
            direction = Direction::approxFromVec(delta);
            animsPlay("attack_" + direction.toString());
            attackState = {};
            break;
        }
    }

    state = newState;
}

//////////////////////////
//        Render        //
//////////////////////////
void EnemyFollower::Render(Vec2<Cart> camera) {
    static int& showDirections = Consts::GetInt("debug.show_directions");
    if (showDirections) {
        auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
        if (!iso) fail("no IsoCollider");
        auto from = iso->box.Center().transmute<Iso>().toCart() - camera;
        auto to = from + moveDelta * 80.0f;

        auto renderer = Game::Instance().Renderer();
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
    }
}

///////////////////////////////////
//        NotifyCollision        //
///////////////////////////////////
void EnemyFollower::NotifyCollision(GameObject& other) {
    auto bullet = (Bullet*)other.GetComponent(CType::Bullet);

    bool bulletHit = bullet && !bullet->TargetsPlayer();
    bool meleeHit = other.tags.test(tag::PlayerHitbox);
    if (bulletHit || meleeHit) {

        // Player stops loosing HP
        Player::player->hpLoss = 0;
        Player::player->hpLossTimer.Restart();

        auto bar =
            (OverheadHpBar*)associated.GetComponent(CType::OverheadHpBar);
        if (bar) {
            bar->SetHp(bar->Hp() - 25);
            associated.RequestAdd(
                MakeHitMarker(25)->WithFootAt(associated.box.Head()));
        }

        // Trauma
        if (meleeHit) {
            Game::Instance().AddTrauma(0.3);
            Game::Instance().Slowdown(0.03, 0.2);
            stunnedLevel += 1.0f;
        }

        if (bar && bar->Hp() <= 0) {
            Die();
            other.RequestDelete();
            return;
        }

        // Flash
        for (auto& sprite : associated.GetAllComponents(CType::Sprite)) {
            ((Sprite*)sprite.get())->WithFlash(0.08);
        }

        // Explosion
        auto hitpoint = other.box.Center();
        hitpoint = hitpoint + Vec2<Cart>{25, 0}.GetRotated(other.angle);
        associated.RequestAdd(MakeExplosion1()->WithCenterAt(hitpoint));

        // Stop attacking
        if (state == Attacking) {
            switchState(Pursuing);
        }

        stunnedLevel += 0.2;

        // Knockback
        float kb = 150'000 * Game::Instance().DeltaTime();
        knockbackVelocity = Vec2<Cart>{kb, 0}.GetRotated(other.angle);

        other.RequestDelete();
    }
}

void EnemyFollower::Die() {
    static int& powerupChance = Consts::GetInt("powerup.chance");
    auto center = associated.box.Center();
    associated.RequestAdd(MakeExplosion4()->WithCenterAt(center));
    associated.RequestDelete();
    if (randi(0, 99) < powerupChance)
        associated.RequestAdd(MakeRandomPowerup()->WithCenterAt(center));

	int coins = randi(0, 5);
	for (int i = 0; i < coins; i++) {
		associated.RequestAdd(MakeCoin()
			->WithCenterAt(center +
				Vec2<Cart>{randi(0,30), randi(0,30)}));
	}
}

///////////////////////////////////
//        Animation utils        //
///////////////////////////////////
void EnemyFollower::animsSoftPlay(const string& id, bool loops) {
    auto& anims = associated.GetAllComponents(CType::Animation);
    for (auto& anim : anims) {
        ((Animation*)anim.get())->SoftPlay(id, loops);
    }
}

void EnemyFollower::animsPlay(const string& id, bool loops) {
    auto& anims = associated.GetAllComponents(CType::Animation);
    for (auto& anim : anims) {
        ((Animation*)anim.get())->Play(id, loops);
    }
}
