#include "State.h"

#include <vector>

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

State::State() : camera(new Camera{}), quitRequested(false) {
    info("initializing");
    LoadAssets();
    music->Play();
    camera->SetSpeed(Vec2{250, 250});
    info("initialized");
}

State::~State() {
    delete music;
    objects.clear();
}

bool State::QuitRequested() { return quitRequested; }

void State::LoadAssets() {
    // Background
    auto bgGO = new GameObject;
    bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    bgGO->AddComponent((Component*)new Sprite{*bgGO, ASSETS "/img/ocean.jpg"});
    bgGO->AddComponent((Component*)new CameraFollower{*bgGO});
    objects.emplace_back(bgGO);

    // Tilemap
    auto tileGO = new GameObject;
    tileGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    auto tileset = new TileSet(DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT,
                               ASSETS "/img/tileset.png");
    auto tilemap = new TileMap(*tileGO, ASSETS "/map/tileMap.txt", tileset);
    tileGO->AddComponent((Component*)tilemap);
    objects.emplace_back(tileGO);

    // Background music
    music = new Music(ASSETS "/audio/stageState.ogg");
}

void State::Update(float dt) {
    camera->Update(dt);

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY))
        quitRequested = true;

    // Create faces on SPACE key press
    if (input.KeyPress(' ')) {
        Vec2 objPos =
            Vec2{200, 0}.GetRotated(-PI + PI * (rng() % 1001) / 500.0) +
            Vec2{(float)input.MouseX(), (float)input.MouseY()};
        AddObject((int)objPos.x, (int)objPos.y);
    }

    for (const auto& go : objects) {
        go->Update(dt);
    }

    // swap-remove dead objects
    for (size_t i = 0; i < objects.size();) {
        if (objects[i]->IsDead()) {
            std::swap(objects[i], objects.back());
            objects.pop_back();
        } else {
            i++;
        }
    }
}

void State::Render() {
    for (const auto& go : objects) {
        go->Render(camera->Pos());
    }
}

void State::AddObject(int mouseX, int mouseY) {
    auto go = new GameObject;

    auto sprite = new Sprite(*go, ASSETS "/img/penguinface.png");
    go->box = Rect{(float)mouseX - sprite->Width() / 2.0f,
                   (float)mouseY - sprite->Height() / 2.0f,
                   (float)sprite->Width(), (float)sprite->Height()};

    auto sound = new Sound(*go, ASSETS "/audio/boom.wav");
    auto face = new Face(*go);

    go->AddComponent((Component*)sprite);
    go->AddComponent((Component*)sound);
    go->AddComponent((Component*)face);
    objects.emplace_back(go);
}
