#include "Prefabs.h"

#include "GameObject.h"
#include "component/IsoCollider.h"
#include "component/Player.h"
#include "component/Sprite.h"
#include "component/enemy/EnemyDistancer.h"
#include "component/enemy/EnemyFollower.h"
#include "component/enemy/RobotCan.h"
#include "util.h"

#define MODULE "Prefabs"

GameObject* MakePlayer() {
    auto go = new GameObject{};
    auto body = new Player{*go};
    go->AddComponent(body);
    go->box.SetCenter(Vec2<Cart>{1300, 900});
    return go;
}

GameObject* MakeEnemyFollower() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(100);
    go->AddComponent(body);
    go->AddComponent((new EnemyFollower{*go})->WithRobotCan(body));
    return go;
}

GameObject* MakeEnemyDistancer() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(300);
    go->AddComponent(body);
    go->AddComponent((new EnemyDistancer{*go})->WithRobotCan(body));
    return go;
}

GameObject* MakeBarril() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/Barris.png"};
    go->AddComponent(sprite);
    go->box.SetFoot(Vec2<Cart>{1300, 600});
    auto collider1 = new IsoCollider{*go};
    collider1->tags.set(tag::Terrain);
    collider1->offset = Rect{261.78, 69.441, 194.642, 130.417};
    auto collider2 = new IsoCollider{*go};
    collider2->tags.set(tag::Terrain);
    collider2->offset = Rect{260.797, 57.3168, 87.1629, 201.196};
    go->AddComponent(collider1);
    go->AddComponent(collider2);
    return go;
}

GameObject* MakeEscavadeira() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/Escavadeira.png"};
    go->AddComponent(sprite);
    auto collider = new IsoCollider{*go};
    collider->tags.set(tag::Terrain);
    collider->offset = Rect{638.71, 138.915, 240.517, 247.071};
    collider->ScaleToSprite();
    collider->ExpandBy(5);
    go->AddComponent(collider);
    return go;
}

