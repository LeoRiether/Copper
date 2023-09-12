#include "PenguinCannon.h"

#include "Bullet.h"
#include "Collider.h"
#include "Component.h"
#include "InputManager.h"
#include "Sprite.h"
#include "util.h"

PenguinCannon::PenguinCannon(GameObject& associated,
                             weak_ptr<GameObject> penguinBody)
    : Component(associated), pbody(penguinBody) {
    auto sprite = new Sprite{associated, ASSETS "/img/cubngun.png"};
    associated.AddComponent(sprite);
    associated.AddComponent(new Collider{associated});
}

void PenguinCannon::Update(float dt) {
    if (pbody.expired()) {
        associated.RequestDelete();
        return;
    }

    timer.Update(dt);

    auto& input = InputManager::Instance();
    auto pb = pbody.lock();

    associated.box.SetCenter(pb->box.Center());

    auto mouse = Vec2{(float)input.MouseX(), (float)input.MouseY()};
    associated.angle = (mouse - associated.box.Center()).angle();

    if (input.IsMouseDown(LEFT_MOUSE_BUTTON) && timer.Get() >= SHOOT_DELAY_S) {
        timer.Restart();
        Shoot();
    }
}

void PenguinCannon::Render(Vec2) {}

bool PenguinCannon::Is(CType type) { return type == CType::PenguinCannon; }

void PenguinCannon::Shoot() {
    float angle = (float)associated.angle;
    int damage = (rng() % 10) + 5;

    auto go = new GameObject{};
    auto bullet = new Bullet{
        *go,  angle, 300, damage, 500, 4, ASSETS "/img/penguinbullet.png",
        false};
    go->AddComponent(bullet);

    // distância do centro do canhão para spawnar a bullet :)
    Vec2 distanceOffset = Vec2{60.0f, 0}.GetRotated(angle);
    go->box.SetCenter(associated.box.Center() + distanceOffset);

    associated.RequestAdd(go);
}
