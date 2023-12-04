#include "physics/CollisionEngine.h"

#include <cmath>

#include "CType.h"
#include "component/Collider.h"
#include "component/Player.h"
#include "math/CohenSutherland.h"
#include "math/Vec2.h"
#include "physics/Collision.h"
#include "physics/IsoSolver.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "CollisionEngine"

vector<IsoCollider*> CollisionEngine::terrainColliders;
unordered_map<chunk_key, vector<IsoCollider*>>
    CollisionEngine::cTerrainColliders;
vector<CollisionEngine::VT> CollisionEngine::vterrainColliders;
vector<CollisionEngine::IG> CollisionEngine::triggers;
GameObject* CollisionEngine::player;
vector<GameObject*> CollisionEngine::entities;
vector<GameObject*> CollisionEngine::bullets;

void CollisionEngine::Update(const vector<shared_ptr<GameObject>>& objects) {
    ClearState();

    player = Player::player ? &Player::player->Associated() : nullptr;

    for (auto& obj : objects) {
        obj->prevFrameBox = obj->box;

        auto& isoColliders = obj->GetAllComponents(CType::IsoCollider);
        for (auto& component : isoColliders) {
            auto isoCollider = (IsoCollider*)component.get();
            auto& tags = isoCollider->tags;

            isoCollider->prevFrameBox = isoCollider->box;

            if (tags.test(tag::Terrain)) {
                terrainColliders.emplace_back(isoCollider);
                auto key = ChunkKey(IsoColliderChunk(isoCollider));
                cTerrainColliders[key].emplace_back(isoCollider);
            }
            if (tags.test(tag::Trigger)) {
                triggers.push_back(IG{isoCollider, obj.get()});
            }
        }

        auto& vterrains = obj->GetAllComponents(CType::Collider);
        for (auto& component : vterrains) {
            auto collider = (Collider*)component.get();
            auto& tags = collider->tags;

            if (tags.test(tag::VTerrain)) {
                vterrainColliders.push_back(VT{collider, obj.get()});
            }
        }

        if (obj->tags.test(tag::Entity)) entities.emplace_back(obj.get());
        if (obj->tags.test(tag::Bullet)) bullets.emplace_back(obj.get());
    }
}

void CollisionEngine::ClearState() {
    terrainColliders.clear();
    cTerrainColliders.clear();
    vterrainColliders.clear();
    triggers.clear();
    player = nullptr;
    entities.clear();
    bullets.clear();
}

// Requires state to be `Update`d!
void CollisionEngine::Solve() {
    // Entity/Player--Terrain collision
    auto processEntity = [&](GameObject* entity) {
        auto entityIso = (IsoCollider*)entity->GetComponent(CType::IsoCollider);
        if (!entityIso) fail("entity/player didn't have IsoCollider");
        chunk2 entityChunk = IsoColliderChunk(entityIso);

        int perfCountTerrains = 0;
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                chunk2 chunk{entityChunk.i + di, entityChunk.j + dj};
                auto before = entityIso->box.TopLeft();
                perfCountTerrains += cTerrainColliders[ChunkKey(chunk)].size();
                for (auto& terrain : cTerrainColliders[ChunkKey(chunk)]) {
                    entityIso->box = IsoSolver::Solve(
                        entityIso->box, entityIso->prevFrameBox, terrain->box);
                }
                auto after = entityIso->box.TopLeft();
                auto delta = (after - before).transmute<Iso>().toCart();
                entity->box.OffsetBy(delta);
            }
        }

        // log2("Processed %d terrains out of %d (%g%%)", perfCountTerrains,
        //      (int)terrainColliders.size(),
        //      double(perfCountTerrains) / terrainColliders.size());
    };

    // Player--Terrain collisions
    if (player) processEntity(player);

    // Entity--Terrain collisions
    for (auto entity : entities) {
        processEntity(entity);
    }

    // Player--Entity and Entity--Entity
    for (int i = (int)entities.size() - 1; i >= 0; i--) {
        auto solve = [&](GameObject* a, GameObject* b) {
            auto a_iso = (IsoCollider*)a->GetComponent(CType::IsoCollider);
            auto b_iso = (IsoCollider*)b->GetComponent(CType::IsoCollider);
            if (!a_iso || !b_iso) {
                warn("[2] entity didn't have IsoCollider!");
                return;
            }

            auto before_a = a_iso->box;
            auto after_a =
                IsoSolver::Solve(a_iso->box, a_iso->prevFrameBox, b_iso->box);
            Vec2<Cart> delta = (after_a.TopLeft() - before_a.TopLeft())
                                   .transmute<Iso>()
                                   .toCart();
            a->box.OffsetBy(delta * 0.5f);
            b->box.OffsetBy(delta * -0.5f);
        };

        if (player) solve(entities[i], player);
        for (int j = i - 1; j >= 0; j--) {
            solve(entities[i], entities[j]);
        }
    }

    // Bullet--Player/Entity/VTerrain
    for (auto bullet : bullets) {
        auto hurtbox = (Collider*)bullet->GetComponent(CType::Collider);

        // --Entity
        for (auto entity : entities) {
            auto hitbox = (Collider*)entity->GetComponent(CType::Collider);
            if (hitbox &&
                Collision::IsColliding(hurtbox->box, hitbox->box, bullet->angle,
                                       entity->angle)) {
                entity->NotifyCollision(*bullet);
            }
        }

        // --Player
        if (player) {
            auto hitbox = (Collider*)player->GetComponent(CType::Collider);
            if (hitbox &&
                Collision::IsColliding(hurtbox->box, hitbox->box, bullet->angle,
                                       player->angle)) {
                player->NotifyCollision(*bullet);
            }
        }

        // --VTerrain
        for (auto& vterrain : vterrainColliders) {
            if (Collision::IsColliding(hurtbox->box, vterrain.c->box,
                                       bullet->angle, vterrain.go->angle)) {
                vterrain.go->NotifyCollision(*bullet);
            }
        }
    }

    // Trigger--Player/Entity
    for (auto& trigger : triggers) {
        auto& tbox = trigger.c->box;
        auto process = [&](GameObject& entity) {
            auto e = (IsoCollider*)entity.GetComponent(CType::IsoCollider);
            if (!e) return;

            if (e->box.CollidesWith(tbox)) {
                trigger.go->NotifyCollision(entity);
                entity.NotifyCollision(*trigger.go);
            }
        };

        if (player) process(*player);
        for (auto entity : entities) {
            process(*entity);
        }
    }
}

bool CollisionEngine::TerrainContains(const Vec2<Iso> point) {
    chunk2 c = Chunk2(point.toCart());
    for (int i = c.i - 1; i <= c.i + 1; i++) {
        for (int j = c.j - 1; j <= c.j + 1; j++) {
            for (auto terrain : cTerrainColliders[ChunkKey({i, j})]) {
                if (terrain->box.Contains(point)) return true;
            }
        }
    }
    return false;
}

bool CollisionEngine::TerrainContainsSegment(const Vec2<Iso> A,
                                             const Vec2<Iso> B) {
    chunk2 cA = Chunk2(A.toCart());
    chunk2 cB = Chunk2(B.toCart());
    chunk2 cmin{std::min(cA.i, cB.i), std::min(cA.j, cB.j)};
    chunk2 cmax{std::max(cA.i, cB.i), std::max(cA.j, cB.j)};

    // they're not actually cartesian, sorry
    const auto At = A.transmute<Cart>();
    const auto Bt = B.transmute<Cart>();

    int perf = 0;
    for (int i = cmin.i - 1; i <= cmax.i + 1; i++) {
        for (int j = cmin.j - 1; j <= cmax.j + 1; j++) {
            // perf += cTerrainColliders[ChunkKey({i, j})].size();
            for (auto terrain : cTerrainColliders[ChunkKey({i, j})]) {
                if (cohen_sutherland::LineClip(At, Bt, terrain->box))
                    return true;
            }
        }
    }

    // log2("checked %d terrains out of %d (%g%%)", perf,
    //      (int)terrainColliders.size(),
    //      (double)perf / (double)terrainColliders.size());

    return false;
}

inline chunk_key CollisionEngine::ChunkKey(chunk2 c) {
    return (chunk_key(c.i) << 32ll) | c.j;
}

inline chunk2 CollisionEngine::Chunk2(Vec2<Cart> pos) {
    return {int(floor(pos.x)) / ChunkSize, int(floor(pos.y)) / ChunkSize};
}

inline chunk2 CollisionEngine::IsoColliderChunk(IsoCollider* iso) {
    return Chunk2(iso->box.Center().transmute<Iso>().toCart());
}
