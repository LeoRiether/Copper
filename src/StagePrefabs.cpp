#include "StagePrefabs.h"

#include "Consts.h"
#include "Game.h"
#include "GameObject.h"
#include "Prefabs.h"
#include "component/EndOfStageTrigger.h"
#include "component/InfiniteBg.h"
#include "component/IsoCollider.h"
#include "component/Sound.h"
#include "component/Tilemap.h"
#include "component/Tileset.h"
#include "math/Rect.h"
#include "util.h"

#define MODULE "StagePrefabs"

struct __Component {
    Vec2<Iso> offset;
    const char* id;
};

void MakeStage1(StageState& s, int stage) {
    constexpr float tilescale = 128.0f;

    vector<__Component> components = {
        {{165, 166}, "bottom of main map"},
        {{132, 133}, "left of main map"},
        {{160, 103}, "top of main map"},
        {{113, 190}, "top left of building map"},
        {{138, 177}, "top right of building map"},
        {{267, 130}, "the one that has a lot of stuff in the center"},
    };

    if (stage < 0) {
        stage = randi(0, components.size() - 1);
    }

    {
        // Fix some coordinates
        auto& c = components[stage];
        c.offset = c.offset + Vec2<Iso>{-2, -8};
        c.offset = c.offset * tilescale;
    }
    auto base = components[stage].offset.toCart();

    //////////////////////////////
    //        Background        //
    //////////////////////////////
    // auto bgGO = new GameObject;
    // bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    // bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/sala_v0.png"});
    // bgGO->renderLayer = -2;
    // RequestAddObject(bgGO);

    // for (auto go : MakeMap1Colliders()) {
    //     RequestAddObject(go);
    // }

    //////////////////////////
    //        Player        //
    //////////////////////////
    auto player = MakePlayer()->WithFootAt(base);
    s.GetCamera().SetPos(player->box.Center());
    s.GetCamera().Follow(player);
    s.RequestAddObject(player);

    ///////////////////////////
    //        Enemies        //
    ///////////////////////////
    s.RequestAddObject(MakeEnemyFollower()->WithFootAt({300, 400}));
    s.RequestAddObject(MakeEnemyDistancer()->WithFootAt({500, 200}));

    ///////////////////////////
    //        Terrain        //
    ///////////////////////////
    auto isoOff = [&](GameObject* go, int xaxis = 1, int yaxis = 0) {
        auto collider = (IsoCollider*)go->GetComponent(CType::IsoCollider);
        auto center = go->box.Center().toIso();
        auto offset =
            Vec2<Iso>{collider->base.w * xaxis, collider->base.h * yaxis};
        go->box.SetCenter((center + offset).toCart());
        return go;
    };

    s.RequestAddObject(MakeBarril()->WithFootAt({-100, -300}));
    s.RequestAddObject(MakeEscavadeira()->WithFootAt({-300, 200}));
    s.RequestAddObject(isoOff(MakeEscavadeira()->WithFootAt({-300, 200})));

    s.RequestAddObject(MakeVigaB()->WithFootAt({400, 100}));
    s.RequestAddObject(isoOff(MakeVigaB()->WithFootAt({400, 100}), 0, 1));
    s.RequestAddObject(isoOff(MakeVigaB()->WithFootAt({400, 100}), 0, 2));
    s.RequestAddObject(isoOff(MakeVigaB()->WithFootAt({400, 100}), 0, 3));

    auto tilemapRenderLayer = -10;
    auto addTilemap = [&](const char* tileset, int tscols, int tsrows,
                          const char* csv, Vec2<Cart> offset, bool colliders,
                          bool mainMap) {
        auto go = new GameObject{};
        go->AddComponent((new Tileset{*go, tileset, tscols, tsrows})
                             ->WithScale(tilescale / 256.0f));
        auto tilemap = (new Tilemap{*go, csv})->WithOffset(offset);
        if (colliders)
            tilemap->WithColliders({883.55, 637.564, 248.128, 251.03});
        go->AddComponent(tilemap);
        go->renderLayer = tilemapRenderLayer++;
        // go->box.SetTopLeft((base * -scale).toCart());
        auto wgo = s.RequestAddObject(go);
        if (mainMap) {
            s.mainMap = wgo;
            tilemap->ComputeComponents();
            for (auto& c : tilemap->Components) {
                info2("component [%d, %d]", c.i, c.j);
            }
        }
    };
    addTilemap(ASSETS "/map/Padrão.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_FLOOR.csv", {0, 0},
               false, true);
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 11.csv",
               {65, -2}, true, false);
    tilemapRenderLayer = 2;
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 13.csv",
               {3, -6}, false, false);
    addTilemap(ASSETS "/map/Aco.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 12.csv",
               {9, 0}, false, false);
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 14.csv",
               {7, 6}, false, false);
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 18.csv",
               {34, 39}, false, false);

    /////////////////////////////////////////
    //        End of Stage Triggers        //
    /////////////////////////////////////////
    {
        auto go = new GameObject{};
        go->AddComponent((new IsoCollider{*go})
                             ->WithTag(tag::Trigger)
                             ->WithBase({19628, 11948, 1162, 522}));
        go->AddComponent(new EndOfStageTrigger{*go});
        s.RequestAddObject(go);
    }

    s.RequestAddObject(MakeStageTransitionDimmer_FadeIn());
}
