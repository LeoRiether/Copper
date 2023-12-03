#include "physics/CollisionEngine.h"

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
    vterrainColliders.clear();
    triggers.clear();
    player = nullptr;
    entities.clear();
    bullets.clear();
}

// Requires state to be `Update`d!
void CollisionEngine::Solve() {
    // Player--Terrain collisions
    if (player) {
        auto playerIso = (IsoCollider*)player->GetComponent(CType::IsoCollider);
        auto before = playerIso->box.TopLeft();
        for (auto& collider : terrainColliders) {
            playerIso->box = IsoSolver::Solve(
                playerIso->box, playerIso->prevFrameBox, collider->box);
        }
        auto after = playerIso->box.TopLeft();
        auto delta = (after - before).transmute<Iso>().toCart();
        player->box.OffsetBy(delta);
    }

    // Entity--Terrain collisions
    for (auto entity : entities) {
        auto entityIso = (IsoCollider*)entity->GetComponent(CType::IsoCollider);
        if (!entityIso) {
            warn("Entity didn't have IsoCollider!");
            continue;
        }
        auto before = entityIso->box.TopLeft();
        for (auto& collider : terrainColliders) {
            entityIso->box = IsoSolver::Solve(
                entityIso->box, entityIso->prevFrameBox, collider->box);
        }
        auto after = entityIso->box.TopLeft();
        auto delta = (after - before).transmute<Iso>().toCart();
        entity->box.OffsetBy(delta);
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
    for (auto terrain : terrainColliders) {
        if (terrain->box.Contains(point)) return true;
    }
    return false;
}

// PERF: sort colliders so we don't have to look at so many of them?
bool CollisionEngine::TerrainContainsSegment(const Vec2<Iso> A,
                                             const Vec2<Iso> B) {
    const auto At = A.transmute<Cart>();
    const auto Bt = B.transmute<Cart>();
    for (auto terrain : terrainColliders) {
        // expand box so enemies don't get stuck on corners as often
        auto box = terrain->box;
        box.x -= 30;
        box.y -= 30;
        box.w += 60;
        box.h += 60;
        if (cohen_sutherland::LineClip(At, Bt, box)) {
            return true;
        }
    }
    return false;
}
