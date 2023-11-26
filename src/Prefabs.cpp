#include "Prefabs.h"

#include <vector>

#include "GameObject.h"
#include "component/Animation.h"
#include "component/Bullet.h"
#include "component/Collider.h"
#include "component/IsoCollider.h"
#include "component/KillTimeout.h"
#include "component/Player.h"
#include "component/Sprite.h"
#include "component/enemy/EnemyDistancer.h"
#include "component/enemy/EnemyFollower.h"
#include "component/enemy/RobotCan.h"
#include "physics/Tags.h"
#include "util.h"

using std::vector;

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
    go->AddComponent(
        (new Collider{*go})->WithBase({18.8157, 3.4533, 32.6644, 76.6754}));
    go->tags.set(tag::Entity);
    return go;
}

GameObject* MakeEnemyDistancer() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(300);
    go->AddComponent(body);
    go->AddComponent((new EnemyDistancer{*go})->WithRobotCan(body));
    go->AddComponent(
        (new Collider{*go})->WithBase({18.8157, 3.4533, 32.6644, 76.6754}));
    go->tags.set(tag::Entity);
    return go;
}

GameObject* MakeBarril() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/Barris.png"};
    go->AddComponent(sprite);
    auto collider1 = new IsoCollider{*go};
    collider1->tags.set(tag::Terrain);
    collider1->base = Rect{261.78, 69.441, 194.642, 130.417};
    auto collider2 = new IsoCollider{*go};
    collider2->tags.set(tag::Terrain);
    collider2->base = Rect{260.797, 57.3168, 87.1629, 201.196};
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
    collider->base = Rect{638.71, 138.915, 240.517, 247.071};
    collider->ScaleToSprite();
    collider->ExpandBy(5);
    go->AddComponent(collider);
    return go;
}

GameObject* MakeBullet(Vec2<Cart> center, float angle) {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/laser-bullet.png"};
    go->AddComponent(new Bullet{*go, 700, angle, 20, 1000, true});
    go->AddComponent(sprite);
    go->AddComponent(
        (new Collider{*go})->WithBase(Rect{46.555, 55.1247, 28.3535, 11.9118}));
    go->box =
        Rect{0, 0, (float)sprite->SheetWidth(), (float)sprite->SheetHeight()};
    go->box.SetCenter(center);
    go->tags.set(tag::Bullet);
    return go;
}

GameObject* MakeVigaB() {
    auto go = new GameObject{};
    go->AddComponent(new Sprite{*go, ASSETS "/img/Viga_B.png"});
    go->AddComponent((new IsoCollider{*go})
                         ->WithBase({666.712, 507.125, 101.171, 147.866})
                         ->WithTag(tag::Terrain));
    return go;
}

GameObject* MakeExplosion1() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/FoozleExplosion.png"};
    auto animation = Animation::horizontal(*go, *sprite, 7, 0.03);
    animation->Play("default", false);  // kickstart box
    go->AddComponent(sprite);
    go->AddComponent(animation);
    go->AddComponent(new KillTimeout{*go, 8 * 0.03});
    go->renderLayer = 2;
    return go;
}

vector<GameObject*> MakeMap1Colliders() {
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

    vector<GameObject*> objects;
    for (int i = 0; i < n; i++) {
        auto go = new GameObject{};
        auto collider = new IsoCollider{*go};
        collider->tags.set(tag::Terrain);
        collider->base = rects[i];
        go->AddComponent(collider);
        objects.push_back(go);
    }
    return objects;
}
