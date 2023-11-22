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
#include "component/enemy/EnemyDistancer.h"
#include "component/enemy/EnemyFollower.h"
#include "component/enemy/RobotCan.h"
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
    go->box.SetCenter(Vec2<Cart>{1300, 900});
    return go;
}

GameObject* StageState::CreateEnemyFollower() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(100);
    go->AddComponent(body);
    go->AddComponent((new EnemyFollower{*go})->WithRobotCan(body));
    go->box.SetFoot(Vec2<Cart>{1700, 400});
    return go;
}

GameObject* StageState::CreateEnemyDistancer() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(300);
    go->AddComponent(body);
    go->AddComponent((new EnemyDistancer{*go})->WithRobotCan(body));
    go->box.SetFoot(Vec2<Cart>{1700, 400});
    return go;
}

void StageState::AddMapColliders() {
    const Rect rects[] = {
        {2098.07, 279.521, 481.1, 561.283},
        {1743.93, 125.836, 672.649, 245.005},
        {1819.65, -105.804, 524.532, 397.576},
        {2027.91, -184.874, 236.095, 169.276},
        {1948.84, -257.262, 315.165, 241.664},
        {2033.48, -337.445, 155.912, 93.5472},
        {2115.89, -455.492, 155.912, 138.093},
        {1706.06, -1039.05, 639.239, 643.694},
        {1907.63, -1374.26, 510.055, 438.781},
        {2217.23, -1585.85, 279.528, 268.391},
        {2339.73, -1579.17, 385.325, 180.412},
        {2706.13, -1495.65, 71.274, 151.457},
        {2794.1, -1472.26, 91.3199, 385.325},
        {2335.28, 737.234, 458.827, 84.6379},
        {2593.65, 826.327, 355.257, 76.8423},
        {2637.08, 898.714, 397.576, 234.982},
        {2776.29, 1049.06, 574.647, 93.5472},
        {3265.18, 983.352, 100.229, 95.7745},
        {3327.55, 969.988, 96.8881, 94.6608},
        {3323.09, 885.35, 85.7516, 83.5243},
        {3244.02, 732.779, 246.118, 83.5243},
        {3338.68, 896.487, 229.413, 82.4106},
        {3438.91, 978.898, 297.346, 90.2062},
        {3632.69, 892.032, 104.684, 100.229},
    };
    const int n = sizeof(rects) / sizeof(*rects);

    for (int i = 0; i < n; i++) {
        auto go = new GameObject{};
        auto collider = new IsoCollider{*go};
        collider->tags.set(tag::Terrain);
        collider->offset = rects[i];
        go->AddComponent(collider);
        RequestAddObject(go);
    }
}

StageState::~StageState() {
    delete music;
    objects.clear();
}

void StageState::Start() {
    LoadAssets();

    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/sala_v0.png"});
    bgGO->renderLayer = -1;
    RequestAddObject(bgGO);

    AddMapColliders();

    // Player
    auto player = CreatePlayer();
    camera->Follow(player);
    RequestAddObject(player);

    // Enemies
    RequestAddObject(CreateEnemyFollower());
    RequestAddObject(CreateEnemyDistancer());

    auto stayingHereForHistorySake = [&]() {
        auto barrel = new GameObject{};
        auto sprite = new Sprite{*barrel, ASSETS "/img/Barris.png"};
        sprite->SetScale(0.2);
        barrel->AddComponent(sprite);
        barrel->box = {0, 0, (float)sprite->SheetWidth() * sprite->Scale().x,
                       (float)sprite->SheetHeight() * sprite->Scale().y};
        barrel->box.SetFoot(Vec2<Cart>{870, 600});
        auto collider1 = new IsoCollider{*barrel};
        collider1->tags.set(tag::Terrain);
        collider1->offset = Rect{1333.5, 605.7, 385, 623};
        collider1->ScaleToSprite();
        collider1->ExpandBy(10);
        collider1->offset.w += 15;
        auto collider2 = new IsoCollider{*barrel};
        collider2->tags.set(tag::Terrain);
        collider2->offset = Rect{1344.7, 465.7, 931, 469};
        collider2->ScaleToSprite();
        barrel->AddComponent(collider1);
        barrel->AddComponent(collider2);
        RequestAddObject(barrel);
    };

    // Barril
    {
        auto barrel = new GameObject{};
        auto sprite = new Sprite{*barrel, ASSETS "/img/Barris.png"};
        barrel->AddComponent(sprite);
        barrel->box.SetFoot(Vec2<Cart>{1300, 600});
        auto collider1 = new IsoCollider{*barrel};
        collider1->tags.set(tag::Terrain);
        collider1->offset = Rect{261.78, 69.441, 194.642, 130.417};
        auto collider2 = new IsoCollider{*barrel};
        collider2->tags.set(tag::Terrain);
        collider2->offset = Rect{260.797, 57.3168, 87.1629, 201.196};
        barrel->AddComponent(collider1);
        barrel->AddComponent(collider2);
        RequestAddObject(barrel);
    }

    // Escavadeira
    {
        auto go = new GameObject{};
        auto sprite = new Sprite{*go, ASSETS "/img/Escavadeira.png"};
        go->AddComponent(sprite);
        go->box.SetFoot(Vec2<Cart>{1200, 700});
        auto collider = new IsoCollider{*go};
        collider->tags.set(tag::Terrain);
        collider->offset = Rect{638.71, 138.915, 240.517, 247.071};
        collider->ScaleToSprite();
        collider->ExpandBy(5);
        go->AddComponent(collider);
        RequestAddObject(go);
    }

    // Escavadeira 2
    {
        auto go = new GameObject{};
        auto sprite = new Sprite{*go, ASSETS "/img/Escavadeira.png"};
        go->AddComponent(sprite);
        go->box.SetFoot(Vec2<Cart>{1200, 700});
        auto collider = new IsoCollider{*go};
        collider->tags.set(tag::Terrain);
        collider->offset = Rect{638.71, 138.915, 240.517, 247.071};
        collider->ScaleToSprite();
        go->AddComponent(collider);
        Vec2<Cart> center = go->box.Center();
        go->box.SetCenter(
            (center.toIso() + Vec2<Iso>{collider->offset.w, 0}).toCart());
        collider->ExpandBy(5);
        RequestAddObject(go);
    }

    // RobotCan
    {
        auto go = new GameObject{};
        auto robotcan = new RobotCan{*go};
        go->AddComponent(robotcan);
        go->box.SetCenter(Vec2<Cart>{1700, 600});
        RequestAddObject(go);
    }

    StartArray();
    started = true;

    // music->Play();
}

void StageState::Pause() {}

void StageState::Resume() {}

void StageState::LoadAssets() {
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
