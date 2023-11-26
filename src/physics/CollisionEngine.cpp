#include "physics/CollisionEngine.h"

#include "CType.h"
#include "component/Collider.h"
#include "component/Player.h"
#include "math/Vec2.h"
#include "physics/Collision.h"
#include "physics/IsoSolver.h"
#include "physics/Tags.h"
#include "util.h"

#define MODULE "CollisionEngine"

vector<IsoCollider*> CollisionEngine::terrainColliders;
GameObject* CollisionEngine::player;
vector<GameObject*> CollisionEngine::entities;
vector<GameObject*> CollisionEngine::bullets;

void CollisionEngine::Update(const vector<shared_ptr<GameObject>>& objects) {
    ClearState();

    player = Player::player ? &Player::player->Associated() : nullptr;

    for (auto& obj : objects) {
        obj->prevFrameBox = obj->box;

        auto isoColliders = obj->GetAllComponents(CType::IsoCollider);
        for (auto component : isoColliders) {
            auto isoCollider = (IsoCollider*)component;
            auto& tags = isoCollider->tags;

            isoCollider->prevFrameBox = isoCollider->box;

            if (tags.test(tag::Terrain)) {
                terrainColliders.emplace_back(isoCollider);
            }
        }

        if (obj->tags.test(tag::Entity)) entities.emplace_back(obj.get());
        if (obj->tags.test(tag::Bullet)) bullets.emplace_back(obj.get());
    }
}

void CollisionEngine::ClearState() {
    terrainColliders.clear();
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

        for (int i = 0; i < 1; i++) {
            for (auto& collider : terrainColliders) {
                playerIso->box = IsoSolver::Solve(
                    playerIso->box, playerIso->prevFrameBox, collider->box);
            }
        }

        auto after = playerIso->box.TopLeft();
        auto movedDelta = (after - before).transmute<Iso>().toCart();
        player->box.SetTopLeft(player->box.TopLeft() + movedDelta);
    }

    // Entity--Terrain collisions
    for (auto entity : entities) {
        auto entityIso = (IsoCollider*)entity->GetComponent(CType::IsoCollider);
        if (!entityIso) {
            warn("Entity didn't have IsoCollider!");
            continue;
        }
        auto before = entityIso->box.TopLeft();

        for (int i = 0; i < 1; i++) {
            for (auto& collider : terrainColliders) {
                entityIso->box = IsoSolver::Solve(
                    entityIso->box, entityIso->prevFrameBox, collider->box);
            }
        }

        auto after = entityIso->box.TopLeft();
        auto movedDelta = (after - before).transmute<Iso>().toCart();
        entity->box.SetTopLeft(entity->box.TopLeft() + movedDelta);
    }

    // Bullet--Player/Entity
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
    }
}

bool CollisionEngine::TerrainContains(const Vec2<Iso> point) {
    for (auto terrain : terrainColliders) {
        auto& box = terrain->box;  // already in Iso I think...
        if (point.x >= box.x && point.x <= box.x + box.w && point.y >= box.y &&
            point.y <= box.y + box.h)
            return true;
    }
    return false;
}
