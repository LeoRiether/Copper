#include "State.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "Alien.h"
#include "CType.h"
#include "CameraFollower.h"
#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "KeepSoundAlive.h"
#include "Rect.h"
#include "Sound.h"
#include "TileMap.h"
#include "TileSet.h"
#include "util.h"

#define MODULE "State"

GameObject* CreatePenguin() {
    auto& input = InputManager::Instance();
    Vec2 objPos = Vec2{200, 0}.GetRotated(-PI + PI * (rng() % 1001) / 500.0) +
                  Vec2{(float)input.MouseX(), (float)input.MouseY()};

    auto go = new GameObject{};
    auto sprite = new Sprite(*go, ASSETS "/img/penguinface.png");
    go->box = Rect{objPos.x, objPos.y, (float)sprite->Width(),
                   (float)sprite->Height()};

    auto sound = new Sound(*go, ASSETS "/audio/boom.wav");

    go->AddComponent(sprite);
    go->AddComponent(sound);
    return go;
}

GameObject* CreateAlien(float x, float y) {
    auto go = new GameObject{};
    auto alien = new Alien{*go, 7};
    auto sprite = (Sprite*)go->GetComponent(CType::Sprite);
    go->box = Rect{x, y, (float)sprite->Width(), (float)sprite->Height()};

    go->AddComponent(alien);
    return go;
}
GameObject* CreateAlien() {
    auto& input = InputManager::Instance();
    Vec2 objPos = Vec2{200, 0}.GetRotated(-PI + PI * (rng() % 1001) / 500.0) +
                  Vec2{(float)input.MouseX(), (float)input.MouseY()};
    return CreateAlien(objPos.x, objPos.y);
}

State::State() : camera(new Camera{}), quitRequested(false), started(false) {
    info("initializing");
    camera->SetSpeed(Vec2{250, 250});
    info("initialized");
}

State::~State() {
    delete music;
    objects.clear();
}

void State::Start() {
    started = true;
    LoadAssets();
    for (auto& go : objects) {
        go->Start();
    }

    AddObject(CreateAlien(512, 300));
    music->Play();
}

bool State::QuitRequested() { return quitRequested; }

void State::LoadAssets() {
    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent(new Sprite{*bgGO, ASSETS "/img/ocean.jpg"});
    bgGO->AddComponent(new CameraFollower{*bgGO});
    AddObject(bgGO);

    // Tilemap
    auto tileGO = new GameObject;
    tileGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    auto tileset = new TileSet(DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT,
                               ASSETS "/img/tileset.png");
    auto tilemap = new TileMap(*tileGO, ASSETS "/map/tileMap.txt", tileset);
    tileGO->AddComponent(tilemap);
    AddObject(tileGO);

    // Background music
    music = new Music(ASSETS "/audio/stageState.ogg");
}

void State::Update(float dt) {
    camera->Update(dt);

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    if (input.KeyPress(' ')) {
        AddObject(CreateAlien());
    }

    currentlyOnUpdate = true;
    for (auto& go : objects) {
        go->Update(dt);
    }
    currentlyOnUpdate = false;

    // swap-remove dead objects
    for (size_t i = 0; i < objects.size();) {
        if (objects[i]->IsDead()) {
            std::swap(objects[i], objects.back());
            objects.pop_back();
        } else {
            i++;
        }
    }

    // add new objects
    size_t n = objects.size();
    for (size_t i = 0; i < n; i++) {
        for (auto go : objects[i]->addRequests) {
            AddObject(go);
        }
        objects[i]->addRequests.clear();
    }
}

void State::Render() {
    for (const auto& go : objects) {
        go->Render(camera->Pos());
    }
}

weak_ptr<GameObject> State::AddObject(GameObject* go) {
    if (currentlyOnUpdate)
        fail("Please don't call AddObject during State::Update!");
    shared_ptr<GameObject> ptr{go};
    objects.push_back(ptr);
    ptr->Start();
    return ptr;
}

// NOTE: why do you need a weak_ptr when you already have a pointer to the
// object anyway?
// WARN: partial function, only call if you're sure the GameObject is in
// `objects`
weak_ptr<GameObject> State::GetObject(GameObject* go) {
    for (auto& candidate : objects) {
        if (go == candidate.get()) return candidate;
    }
    fail("GetObject called with GameObject that's not registered");
}
