#include "StagePrefabs.h"

#include <functional>
#include <unordered_map>

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
    const char* id;
    const char* description;
    Vec2<Iso> offset;
};

using ObjectGenerator = std::function<vector<GameObject*>()>;
using std::string;

void MakeStage1(StageState& s, int stage) {
    constexpr float tilescale = 128.0f;

    // Translates from "tiled coordinates" to cartesian screen coordinates
    auto worldPos = [&](Vec2<Iso> isoPos) {
        return ((isoPos + Vec2<Iso>{-2, -8}) * tilescale).toCart();
    };

    auto isoOff = [&](GameObject* go, int xaxis = 1, int yaxis = 0) {
        auto collider = (IsoCollider*)go->GetComponent(CType::IsoCollider);
        auto center = go->box.Center().toIso();
        auto offset =
            Vec2<Iso>{collider->base.w * xaxis, collider->base.h * yaxis};
        go->box.SetCenter((center + offset).toCart());
        return go;
    };

    vector<__Component> components = {
        {"main", "bottom of main map", {165, 166}},
        {"main", "left of main map", {132, 133}},
        {"main", "top of main map", {160, 103}},
        {"building", "top left of building map", {113, 190}},
        {"building", "top right of building map", {138, 177}},
        {"center", "the one that has a lot of stuff in the center", {267, 130}},
    };

    std::unordered_map<string, ObjectGenerator> generators{
        {"main",
         [&]() {
             GameObject* go;
             return vector<GameObject*>{
                 // Terrain
                 MakeBarril()->WithFootAt(worldPos({149, 135})),
                 MakeEscavadeira()->WithFootAt(worldPos({156, 148})),
                 MakeEscavadeira()->WithFootAt(worldPos({160, 148})),
                 MakeVigaB()->WithFootAt(worldPos({162, 133})),
                 MakeVigaB()->WithFootAt(worldPos({162, 132})),
                 MakeVigaB()->WithFootAt(worldPos({162, 131})),
                 MakeVigaB()->WithFootAt(worldPos({162, 130})),

                 // Enemies
                 MakeEnemyFollower()->WithFootAt(worldPos({149, 127})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({150, 146})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({172, 145})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({170, 117})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({169, 117})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({170, 116})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({179, 124})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({178, 124})),
                 MakeEnemyDistancer()->WithFootAt(worldPos({179, 123})),

                 // End of stage triggers are kind of weird sorry
                 (go = new GameObject{})
                     ->AddComponent((new IsoCollider{*go})
                                        ->WithTag(tag::Trigger)
                                        ->WithBase({19628, 11948, 1162, 522}))
                     ->AddComponent(new EndOfStageTrigger{*go}),
             };
         }},
    };

    if (stage < 0) {
        stage = randi(0, components.size() - 1);
    }

    // Fix some coordinates
    auto& c = components[stage];
    c.offset = c.offset + Vec2<Iso>{-2, -8};
    c.offset = c.offset * tilescale;
    auto base = components[stage].offset.toCart();

    //////////////////////////////
    //        Background        //
    //////////////////////////////
    // auto bgGO = new GameObject;
    // bgGO->box = Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    // bgGO->AddComponent(new InfiniteBg{*bgGO, ASSETS "/img/sala_v0.png"});
    // bgGO->renderLayer = -2;
    // RequestAddObject(bgGO);

    //////////////////////////
    //        Player        //
    //////////////////////////
    auto player = MakePlayer()->WithFootAt(base);
    s.GetCamera().SetPos(player->box.Center());
    s.GetCamera().Follow(player);
    s.RequestAddObject(player);
    s.RequestAddObject(
        MakeCompanion()->WithFootAt(base - Vec2<Cart>{-100, -100}));

    if (generators.count(c.id)) {
        for (auto go : generators[c.id]()) {
            s.RequestAddObject(go);
        }
    }

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

    s.RequestAddObject(MakeStageTransitionDimmer_FadeIn());
}
