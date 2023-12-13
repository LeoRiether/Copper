#include "StagePrefabs.h"

#include <functional>
#include <unordered_map>

#include "Consts.h"
#include "Game.h"
#include "GameObject.h"
#include "Prefabs.h"
#include "component/DialogTrigger.h"
#include "component/EndOfStageTrigger.h"
#include "component/InfiniteBg.h"
#include "component/IsoCollider.h"
#include "component/Sound.h"
#include "component/Tilemap.h"
#include "component/Tileset.h"
#include "math/Rect.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "StagePrefabs"

using ObjectGenerator = std::function<vector<GameObject*>()>;

struct __Component {
    Vec2<Iso> offset;
    ObjectGenerator gen;
};

using std::string;

void MakeStage1(StageState& s, string stage) {
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

    // min enemies
    std::unordered_map<string, int> minEnemy;
    minEnemy["main"] = 4;
    minEnemy["the one on the top left"] = 3;

    std::unordered_map<string, __Component> components;
    components["main"] = {
        {165, 166},
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
                MakeEnemyFollower()->WithFootAt(worldPos({150, 127})),
                MakeEnemyFollower()->WithFootAt(worldPos({151, 127})),
                MakeEnemyDistancer()->WithFootAt(worldPos({150, 146})),
                MakeEnemyDistancer()->WithFootAt(worldPos({172, 145})),
                MakeEnemyDistancer()->WithFootAt(worldPos({170, 117})),
                MakeEnemyDistancer()->WithFootAt(worldPos({169, 117})),
                MakeEnemyDistancer()->WithFootAt(worldPos({170, 116})),
                MakeEnemyDistancer()->WithFootAt(worldPos({179, 124})),
                MakeEnemyDistancer()->WithFootAt(worldPos({178, 124})),
                MakeEnemyDistancer()->WithFootAt(worldPos({179, 123})),

				//Dialog triggers
				
				MakeDialogTrigger({19765, 19447, 200, 200}
						, ASSETS "/dialog/sample.txt"),
				MakeDialogTrigger({21177, 19939, 200, 200}
						, ASSETS "/dialog/other.txt"),

                // End of stage triggers are kind of weird sorry
                (go = new GameObject{})
                    ->AddComponent((new IsoCollider{*go})
                                       ->WithTag(tag::Trigger)
                                       ->WithBase({19628, 11948, 1162, 522}))
                    ->AddComponent(new EndOfStageTrigger{*go}),
            };
        },
    };
    components["the one on the top left"] = {
        {14, 154},
        [&]() {
            GameObject* go;
            return vector<GameObject*>{
                MakeEnemyDistancer()->WithFootAt(worldPos({12, 123})),
                MakeEnemyDistancer()->WithFootAt(worldPos({13, 123})),
                MakeEnemyDistancer()->WithFootAt(worldPos({14, 123})),
                MakeEnemyDistancer()->WithFootAt(worldPos({12, 125})),
                MakeEnemyDistancer()->WithFootAt(worldPos({13, 126})),
                MakeEnemyDistancer()->WithFootAt(worldPos({14, 127})),

                (go = new GameObject{})
                    ->AddComponent((new IsoCollider{*go})
                                       ->WithTag(tag::Trigger)
                                       ->WithBase({1191, 12765, 2043 - 1191,
                                                   13585 - 12765}))
                    ->AddComponent(new EndOfStageTrigger{*go}),

            };
        },
    };

    vector<string> componentIds;
    componentIds.reserve(components.size());
    for (auto& [key, _] : components) {
        componentIds.emplace_back(key);
    }

    if (stage == "") stage = componentIds[randi(0, components.size() - 1)];
    log2("Chose stage '%s'", stage.c_str());

    // Select enemies (its rly ugly sry)
    int enemyNumber = 0;
    for (auto go : components[stage].gen()) {
        if (go->tags.test(tag::Enemy)) {enemyNumber++;}
    }

    vector<int> enemySelected;
    int count = 0;
    for (int i=0; i<enemyNumber; i++) {
        int j = randi(0, 1);
        if (j) {count++;}
        else {
            if(minEnemy[stage]-count > enemyNumber-i) {
                count++;
                j = 1;
            }
        }
        enemySelected.emplace_back(j);
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

    int i = 0;
    for (auto go : c.gen()) {

        if (go->tags.test(tag::Enemy)) {
            if (!enemySelected[i]) {
                delete go;
            } else {
                s.RequestAddObject(go));
                s.EnemyCount++;
            }
            i++;
        } else {
            s.RequestAddObject(go);
        }
    }

    auto tilemapRenderLayer = -10;
    auto addTilemap = [&](const char* tileset, int tscols, int tsrows,
                          const char* csv, Vec2<Cart> offset, bool colliders,
                          bool mainMap, bool floorColliders) {
        auto go = new GameObject{};
        go->AddComponent((new Tileset{*go, tileset, tscols, tsrows})
                             ->WithScale(tilescale / 256.0f));
        auto tilemap = (new Tilemap{*go, csv})->WithOffset(offset);
        if (colliders)
            tilemap->WithColliders({884.419, 625.152, 265.792, 266.743});
        if (floorColliders)
            tilemap->WithFloorColliders({884.419, 625.152, 265.792, 266.743});
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
               false, true, true);
    tilemapRenderLayer = 0;
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 11.csv",
               {65, -2}, true, false, false);
    tilemapRenderLayer = 2;
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 13.csv",
               {3, -6}, false, false, false);
    addTilemap(ASSETS "/map/Aco.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 12.csv",
               {9, 0}, false, false, false);
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 14.csv",
               {7, 6}, false, false, false);
    addTilemap(ASSETS "/map/Ferrugem.png", 13, 19,
               ASSETS "/map/Salas copper V2_Copy of Group 2_Tile Layer 18.csv",
               {34, 39}, false, false, false);

    s.RequestAddObject(MakeStageTransitionDimmer_FadeIn());
}
