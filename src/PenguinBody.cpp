#include "PenguinBody.h"

#include "Game.h"
#include "GameObject.h"
#include "PenguinCannon.h"
#include "Sprite.h"
#include "util.h"

PenguinBody* PenguinBody::player;

PenguinBody::PenguinBody(GameObject& associated) : Component(associated) {
    PenguinBody::player = this;

    auto sprite = new Sprite{associated, ASSETS "/img/penguin.png"};
    associated.AddComponent(sprite);
}

PenguinBody::~PenguinBody() { PenguinBody::player = nullptr; }

void PenguinBody::Start() {
    auto& state = Game::Instance().GetState();
    auto go = new GameObject{};
    auto cannon = new PenguinCannon{*go, state.GetObject(&associated)};
    associated.RequestAdd(go);
}

void PenguinBody::Update(float dt) {}

void PenguinBody::Render(Vec2) {}

bool PenguinBody::Is(CType type) { return type == CType::PenguinBody; }
