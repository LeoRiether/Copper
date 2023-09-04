#include "State.h"

#include <vector>

#include "GameObject.h"
#include "KeepSoundAlive.h"
#include "Rect.h"
#include "Sound.h"
#include "util.h"

#define MODULE "State"

State::State() : quitRequested(false) {
    info("initializing");
    LoadAssets();
    music->Play();
    info("initialized");
}

State::~State() {
    delete music;
    objects.clear();
}

bool State::QuitRequested() { return quitRequested; }

void State::LoadAssets() {
    auto go = new GameObject;
    go->box = Rect{0, 0, 0, 0};
    go->AddComponent((Component*)new Sprite(*go, ASSETS "/img/ocean.jpg"));
    objects.emplace_back(go);

    music = new Music(ASSETS "/audio/stageState.ogg");
}

void State::Update(float dt) {
    Input();
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
        go->Render();
    }
}

void State::Input() {
    SDL_Event event;
    int mouseX, mouseY;

    // Obtenha as coordenadas do mouse
    SDL_GetMouseState(&mouseX, &mouseY);

    // SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
    while (SDL_PollEvent(&event)) {
        // Se o evento for quit, setar a flag para terminação
        if (event.type == SDL_QUIT) {
            quitRequested = true;
        }

        // Se o evento for clique...
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Percorrer de trás pra frente pra sempre clicar no objeto mais de
            // cima
            for (int i = objects.size() - 1; i >= 0; --i) {
                const auto& go = objects[i];

                if (go->box.Contains({(float)mouseX, (float)mouseY})) {
                    Face* face = (Face*)go->GetComponent(CType::Face);
                    if (face != nullptr) {
                        // Aplica dano
                        face->Damage(std::rand() % 10 + 10);
                        // Sai do loop (só queremos acertar um)
                        break;
                    }
                }
            }
        }
        if (event.type == SDL_KEYDOWN) {
            // Se a tecla for ESC, setar a flag de quit
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                quitRequested = true;
            }
            // Se não, crie um objeto
            else {
                Vec2 objPos =
                    Vec2{200, 0}.GetRotated(-PI + PI * (rng() % 1001) / 500.0) +
                    Vec2{(float)mouseX, (float)mouseY};
                AddObject((int)objPos.x, (int)objPos.y);
            }
        }
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
