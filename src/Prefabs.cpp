#include "Prefabs.h"

#include <vector>

#include "Colors.h"
#include "GameObject.h"
#include "component/Animation.h"
#include "component/Bar.h"
#include "component/Bullet.h"
#include "component/BulletShaker.h"
#include "component/Collider.h"
#include "component/Companion.h"
#include "component/ControlsTutorial.h"
#include "component/IsoCollider.h"
#include "component/KeepSoundAlive.h"
#include "component/KillTimeout.h"
#include "component/LifeBarManager.h"
#include "component/Player.h"
#include "component/Sound.h"
#include "component/Sprite.h"
#include "component/StageTransitionDimmer.h"
#include "component/Text.h"
#include "component/enemy/EnemyDistancer.h"
#include "component/enemy/EnemyFollower.h"
#include "component/enemy/RobotCan.h"
#include "physics/Tags.h"
#include "util.h"

using std::vector;

#define MODULE "Prefabs"

GameObject* MakePlayer() {
    auto go = new GameObject{};
    go->tags.set(tag::Player);
    go->AddComponent(new Player{*go});
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Player)
                         ->WithBase({681.329, 425.945, 107.613, 107.613})
                         ->ScaleToSprite());
    go->AddComponent((new Collider{*go})
                         ->WithBase({75.5414, 104.845, 127.273, 231.782})
                         ->ScaleToSprite());
    go->AddComponent((new Sound{*go, ASSETS "/audio/Gravel - Run.wav", 90}));
    return go;
}

GameObject* MakeCompanion() {
    auto go = new GameObject{};
    go->AddComponent(new RobotCan{*go});
    go->AddComponent(new Companion{*go});
    go->AddComponent(
        (new Collider{*go})->WithBase({18.8157, 3.4533, 32.6644, 76.6754}));
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Entity)
                         ->WithBase({147.784, 76.4568, 51.9192, 52.6069}));
    go->tags.set(tag::Entity);
    return go;
}

GameObject* MakeControlsTutorial() {
    auto go = new GameObject{};
    go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 25,
                              Text::Wrapped, "?", colorFromHex("4D2011")})
        ->AddComponent(new ControlsTutorial{*go});
    go->renderLayer = 15;
    return go;
}

GameObject* MakeEnemyFollower() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(100);
    go->AddComponent(body);
    go->AddComponent((new EnemyFollower{*go})->WithRobotCan(body));
    go->AddComponent(
        (new Collider{*go})->WithBase({18.8157, 3.4533, 32.6644, 76.6754}));
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Entity)
                         ->WithBase({147.784, 76.4568, 51.9192, 52.6069}));
    go->tags.set(tag::Entity);
    go->tags.set(tag::Enemy);
    return go;
}

GameObject* MakeEnemyDistancer() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(300);
    go->AddComponent(body);
    go->AddComponent((new EnemyDistancer{*go})->WithRobotCan(body));
    go->AddComponent(
        (new Collider{*go})->WithBase({18.8157, 3.4533, 32.6644, 76.6754}));
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Entity)
                         ->WithBase({147.784, 76.4568, 51.9192, 52.6069}));
    go->tags.set(tag::Entity);
    go->tags.set(tag::Enemy);
    return go;
}

GameObject* MakeBarril() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/Barris.png"};
    go->AddComponent(sprite);
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Terrain)
                         ->WithBase({338.457, 91.8543, 110.887, 109.314}));
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Terrain)
                         ->WithBase({260.338, 86.0871, 82.0511, 166.461}));
    go->AddComponent((new Collider{*go})
                         ->WithTag(tag::VTerrain)
                         ->WithBase({74.5427, 2.52368, 96.0102, 154.993}));
    go->AddComponent((new Collider{*go})
                         ->WithTag(tag::VTerrain)
                         ->WithBase({1.47008, 42.173, 79.2986, 103.219}));
    go->AddComponent((new BulletShaker{*go}));
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
    go->AddComponent(collider);
    return go;
}

GameObject* MakeBullet(Vec2<Cart> center, float angle) {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/laser-bullet.png"};
    go->AddComponent(new Bullet{*go, 700, angle, 20, 4000, true});
    go->AddComponent(sprite);
    go->AddComponent(
        (new Collider{*go})->WithBase(Rect{46.555, 55.1247, 28.3535, 11.9118}));
    go->box =
        Rect{0, 0, (float)sprite->SheetWidth(), (float)sprite->SheetHeight()};
    go->box.SetCenter(center);
    go->tags.set(tag::Bullet);
    return go;
}

GameObject* MakePlayerBullet(Vec2<Cart> center, float angle) {
    // Code reuse? Where we're going, we don't need code reuse
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/yellow-laser-bullet.png"};
    go->AddComponent(new Bullet{*go, 700, angle, 20, 4000, false});
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
    auto animation = Animation::horizontal(*go, *sprite, 7, 0.06);
    animation->Play("default", false);  // kickstart box
    go->AddComponent(sprite);
    go->AddComponent(animation);
    go->AddComponent(new KillTimeout{*go, 8 * 0.09});
    go->renderLayer = 2;
    return go;
}

GameObject* MakeExplosion3() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/explosion-3.png"};
    auto animation = Animation::horizontal(*go, *sprite, 10, 0.09);
    animation->Play("default", false);  // kickstart box
    go->AddComponent(sprite);
    go->AddComponent(animation);
    go->AddComponent(new KillTimeout{*go, 10 * 0.09});
    go->renderLayer = 2;
    return go;
}

GameObject* MakeExplosion4() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/explosion-4.png"};
    auto animation = Animation::horizontal(*go, *sprite, 12, 0.09);
    animation->Play("default", false);  // kickstart box
    go->AddComponent(sprite);
    go->AddComponent(animation);
    go->AddComponent(new KillTimeout{*go, 12 * 0.09});
    go->renderLayer = 2;
    return go;
}

GameObject* MakeStageTransitionDimmer_FadeIn() {
    auto go = new GameObject{};
    go->AddComponent((new StageTransitionDimmer{*go, 0, 255, 0.7})
                         ->WithEasing(StageTransitionDimmer::In));
    return go;
}
GameObject* MakeStageTransitionDimmer_FadeOut() {
    auto go = new GameObject{};
    go->AddComponent((new StageTransitionDimmer{*go, 255, 0, 0.7})
                         ->WithEasing(StageTransitionDimmer::Out));
    return go;
}

GameObject* MakeOneOffAudio(std::string file, int volume) {
    auto go = new GameObject{};
    auto sound = new Sound{*go, file, volume};
    sound->Play();
    return go->AddComponent(sound)->AddComponent(new KeepSoundAlive{*go});
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

GameObject* MakeLifeBar() {
    auto go = new GameObject{};
    int maxBar = 10;
    Vec2<Cart> dimension = {412, 144};
    int space = 20;
    auto lifeBar =
        new Bar{*go, "assets/img/Lifebar.png", maxBar, dimension, space};
    lifeBar->SetBarState(maxBar);
    go->box.SetCenter(Vec2<Cart>{10, 10});
    go->renderLayer = 200;
    auto hpManager = new LifeBarManager(*go, 100, lifeBar);

    go->AddComponent(hpManager);

    return go;
}
