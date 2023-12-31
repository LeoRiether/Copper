#include "Prefabs.h"

#include <vector>

#include "Colors.h"
#include "GameObject.h"
#include "component/Animation.h"
#include "component/Bar.h"
#include "component/Bullet.h"
#include "component/BulletShaker.h"
#include "component/Coin.h"
#include "component/Collider.h"
#include "component/Companion.h"
#include "component/ControlsTutorial.h"
#include "component/Dialog.h"
#include "component/DialogTrigger.h"
#include "component/Exploder.h"
#include "component/HitMarker.h"
#include "component/IsoCollider.h"
#include "component/KeepSoundAlive.h"
#include "component/KillTimeout.h"
#include "component/LifeBarManager.h"
#include "component/OverheadHpBar.h"
#include "component/Pickupable.h"
#include "component/Player.h"
#include "component/Slash.h"
#include "component/Sound.h"
#include "component/Sprite.h"
#include "component/StageTransitionDimmer.h"
#include "component/Text.h"
#include "component/enemy/EnemyDistancer.h"
#include "component/enemy/EnemyFollower.h"
#include "component/enemy/RobotCan.h"
#include "component/powerup/DoubledBullets.h"
#include "component/powerup/Speedy.h"
#include "component/powerup/StrongerAttack.h"
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
                         ->WithBase({368.171, 138.745, 124.892, 122.601})
                         ->ScaleToSprite());
    go->AddComponent((new Collider{*go})
                         ->WithBase({97.716, 56.2834, 41.2575, 101.234})
                         ->ScaleToSprite());
    go->AddComponent((new Sound{*go, ASSETS "/audio/Gravel - Run.wav", 90}));
    return go;
}

GameObject* MakeCompanion() {
    auto go = new GameObject{};
    go->AddComponent(new Companion{*go});
    go->AddComponent(
        (new Collider{*go})->WithBase({15.6793, 22.2168, 39.7406, 56.9443}));
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Entity)
                         ->WithBase({159.529, 87.2253, 59.6754, 59.6754}));
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
    go->AddComponent(new OverheadHpBar{*go, 150, 150});
    go->AddComponent(new EnemyFollower{*go});
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Entity)
                         ->WithBase({390.975, 153.522, 45.8416, 45.8416})
                         ->ScaleToSprite());
    go->AddComponent((new Collider{*go})
                         ->WithBase({97.716, 56.2834, 41.2575, 101.234})
                         ->ScaleToSprite());
    go->tags.set(tag::Entity);
    go->tags.set(tag::Enemy);
    return go;
}

GameObject* MakeEnemyDistancer() {
    auto go = new GameObject{};
    auto body = (new RobotCan{*go})->WithStopDistance(300);
    go->AddComponent(body);
    go->AddComponent(new OverheadHpBar{*go, 100, 100});
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
    go->box.w = sprite->SheetWidth();
    go->box.h = sprite->SheetHeight();
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
    go->AddComponent(new OverheadHpBar{*go, 150, 150});
    go->AddComponent(new Exploder{*go});
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

GameObject* MakeExplosion3(Vec2<Cart> scale) {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/explosion-3.png"};
    sprite->SetScale(scale);
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

GameObject* MakeSlash(Vec2<Cart> center, float angle) {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/slash.png"};
    auto animation = Animation::horizontal(*go, *sprite, 5, 0.05);
    go->AddComponent(sprite)
        ->AddComponent(animation)
        ->AddComponent(new KillTimeout{*go, 5 * 0.05})
        ->AddComponent(new Slash{*go, angle});
    animation->Play("default");
    go->box.SetCenter(center);
    go->angle = angle;
    go->renderLayer = 3;
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

GameObject* MakeDialog(std::string dialogFile) {
    auto go = new GameObject{};
    auto dialog = new Dialog{*go, dialogFile};
    go->AddComponent(dialog);
    go->renderLayer = 201;
    return go;
}

GameObject* MakeDialogTrigger(Rect base, std::string dialogFile) {
    auto go = new GameObject{};
    go->AddComponent(
        (new IsoCollider{*go})->WithTag(tag::Trigger)->WithBase(base));
    go->AddComponent(new DialogTrigger(*go, dialogFile));
    go->renderLayer = 100;
    return go;
}

GameObject* MakeRandomPowerup() {
    auto go = new GameObject{};
    int roll = randi(0, 2);
    if (roll == 0)
        go->AddComponent(new DoubledBullets{*go});
    else if (roll == 1)
        go->AddComponent(new Speedy{*go});
    else if (roll == 2)
        go->AddComponent(new StrongerAttack{*go});
    go->AddComponent(new Pickupable{*go});
    go->AddComponent((new IsoCollider{*go})
                         ->WithTag(tag::Trigger)
                         ->WithBase({80.415, 13.9493, 91.6036, 92.9458}));
    return go;
}

GameObject* MakeHitMarker(int dmg) {
    auto go = new GameObject{};
    go->AddComponent(new HitMarker{*go, dmg});
    return go;
}

GameObject* MakeCoin() {
    auto coin = new GameObject{};
    auto coinSprite = new Sprite{*coin, ASSETS "/img/lego_studs.png"};
    coinSprite->SetScale(0.5);
    auto animation = Animation::horizontal(*coin, *coinSprite, 32, 0.05);
    animation->Play("default", true);
    coin->AddComponent(coinSprite);
    coin->AddComponent(animation);
    coin->AddComponent(new Coin{*coin});
    coin->AddComponent((new IsoCollider{*coin})
                           ->WithTag(tag::Trigger)
                           ->WithBase({50, 20.9493, 60, 50}));
    return coin;
}

GameObject* MakeC1() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/props/c1.png"};
    sprite->SetScale(0.5f);
    go->AddComponent(sprite);
    go->AddComponent(
        (new IsoCollider{*go})->WithBase({ 448, 72, 321, 379 })->WithTag(tag::Terrain)->ScaleToSprite());
    return go;
}

GameObject* MakeC2() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/props/c2.png"};
    sprite->SetScale(0.5f);
    go->AddComponent(sprite);
    go->AddComponent(
        (new IsoCollider{*go})->WithBase({ 448, 72, 321, 379 })->WithTag(tag::Terrain)->ScaleToSprite());
    return go;
}

GameObject* MakeC3() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/props/c3.png"};
    sprite->SetScale(0.5f);
    go->AddComponent(sprite);
    go->AddComponent(
        (new IsoCollider{*go})->WithBase({ 448, 72, 321, 379 })->WithTag(tag::Terrain)->ScaleToSprite());
    return go;
}

GameObject* MakeBarril2() {
    auto go = new GameObject{};
    auto sprite = new Sprite{*go, ASSETS "/img/props/b2.png"};
    go->box.w = sprite->SheetWidth();
    go->box.h = sprite->SheetHeight();
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
    go->AddComponent(new OverheadHpBar{*go, 150, 150});
    go->AddComponent(new Exploder{*go});
    return go;
}
