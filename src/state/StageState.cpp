#include "state/StageState.h"

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "CType.h"
#include "Consts.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "component/CameraFollower.h"
#include "component/Collider.h"
#include "component/EndStateDimmer.h"
#include "component/InfiniteBg.h"
#include "component/IsoCollider.h"
#include "component/KeepSoundAlive.h"
#include "component/Player.h"
#include "component/Sound.h"
#include "component/enemy/Enemy.h"
#include "component/enemy/EnemyDistancer.h"
#include "component/enemy/EnemyFollower.h"
#include "math/Rect.h"
#include "physics/Collision.h"
#include "physics/CollisionEngine.h"
#include "state/TitleState.h"
#include "util.h"

#define MODULE "StageState"

GameObject* StageState::CreatePlayer() {
    auto go = new GameObject{};
    auto body = new Player{*go};
    go->AddComponent(body);
    go->box.SetCenter(Vec2<Cart>{704, 640});
    return go;
}

GameObject* StageState::CreateEnemyFollower() {
    auto go = new GameObject{};
    auto body = new Enemy{*go};
    (*body).WithStopDistance(100).WithBehavior(new EnemyFollower);
    go->AddComponent(body);
    go->box.SetFoot(Vec2<Cart>{1000, 400});
    return go;
}

GameObject* StageState::CreateEnemyDistancer() {
    auto go = new GameObject{};
    auto body = new Enemy{*go};
    (*body).WithStopDistance(300).WithBehavior(new EnemyDistancer);
    go->AddComponent(body);
    go->box.SetFoot(Vec2<Cart>{1000, 400});
    return go;
}

StageState::~StageState() {
    delete music;
    objects.clear();
}

void StageState::Start() {
    LoadAssets();

    // Player
    auto player = CreatePlayer();
    camera->Follow(player);
    RequestAddObject(player);

    // Enemies
    RequestAddObject(CreateEnemyFollower());
    RequestAddObject(CreateEnemyDistancer());

    // Barril
    {
        auto barrel = new GameObject{};
        auto sprite = new Sprite{*barrel, ASSETS "/img/Barris.png"};
        sprite->SetScale(0.2);
        barrel->AddComponent(sprite);
        barrel->box = {0, 0, (float)sprite->SheetWidth() * sprite->Scale().x,
                       (float)sprite->SheetHeight() * sprite->Scale().y};
        barrel->box.SetFoot(Vec2<Cart>{870, 600});
        auto collider1 = new IsoCollider{*barrel};
        collider1->tags.set(tag::Terrain);
        collider1->offset = Rect{834.04, 113.16, 338.208, 336.592};
        collider1->ScaleToSprite();
        auto collider2 = new IsoCollider{*barrel};
        collider2->tags.set(tag::Terrain);
        collider2->offset = Rect{640.039, 192.361, 245.905, 420.495};
        collider2->ScaleToSprite();
        barrel->AddComponent(collider1);
        barrel->AddComponent(collider2);
        RequestAddObject(barrel);
    }

    // Escavadeira
    {
        auto go = new GameObject{};
        auto sprite = new Sprite{*go, ASSETS "/img/Escavadeira.png"};
        sprite->SetScale(0.2);
        go->AddComponent(sprite);
        go->box = {0, 0, (float)sprite->SheetWidth() * sprite->Scale().x,
                   (float)sprite->SheetHeight() * sprite->Scale().y};
        go->box.SetFoot(Vec2<Cart>{1200, 700});
        auto collider = new IsoCollider{*go};
        collider->tags.set(tag::Terrain);
        collider->offset = Rect{1555.78, 134.273, 754.746, 754.746};
        collider->ScaleToSprite();
        collider->ExpandBy(5);
        go->AddComponent(collider);
        RequestAddObject(go);
    }

    // Escavadeira 2
    {
        auto go = new GameObject{};
        auto sprite = new Sprite{*go, ASSETS "/img/Escavadeira.png"};
        sprite->SetScale(0.2);
        go->AddComponent(sprite);
        go->box = {0, 0, (float)sprite->SheetWidth() * sprite->Scale().x,
                   (float)sprite->SheetHeight() * sprite->Scale().y};
        go->box.SetFoot(Vec2<Cart>{1200, 700});
        auto collider = new IsoCollider{*go};
        collider->tags.set(tag::Terrain);
        collider->offset = Rect{1555.78, 134.273, 754.746, 754.746};
        collider->ScaleToSprite();
        go->AddComponent(collider);
        Vec2<Cart> center = go->box.Center();
        go->box.SetCenter(
            (center.toIso() + Vec2<Iso>{collider->offset.w, 0}).toCart());
        collider->ExpandBy(5);
        RequestAddObject(go);
    }

    StartArray();
    started = true;

    // music->Play();
}

void StageState::Pause() {}

void StageState::Resume() {}

void StageState::LoadAssets() {
    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/isobg.png"});
    bgGO->renderLayer = -1;
    RequestAddObject(bgGO);

    // Background music
    music = new Music(ASSETS "/audio/stageState.ogg");
}

void StageState::Update(float dt) {
    // process add requests here so added objects are updated before their first
    // render
    ProcessAddRequests();

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested()) {
        quitRequested = true;
    }

    if (input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        Game::Instance().RequestPop();
        return;
    }

    if (input.KeyPress(SDL_SCANCODE_C)) {
        Consts::GetInt("debug.show_colliders") ^= 1;
    }

    // Handle updates
    for (auto& go : objects) {
        go->Update(dt);
    }
    CollisionEngine::Solve(objects);
    camera->Update(dt);

    // Swap-remove dead objects
    for (size_t i = 0; i < objects.size();) {
        if (objects[i]->IsDead()) {
            if (camera->Focus() == objects[i].get()) camera->Unfollow();
            std::swap(objects[i], objects.back());
            objects.pop_back();
        } else {
            i++;
        }
    }
}

void StageState::Render() { RenderArray(); }
