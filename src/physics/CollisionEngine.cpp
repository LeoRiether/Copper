#include "physics/CollisionEngine.h"

#include "CType.h"
#include "component/Collider.h"
#include "component/Player.h"
#include "physics/Collision.h"
#include "physics/IsoSolver.h"
#include "physics/Tags.h"

#define MODULE "CollisionEngine"

vector<IsoCollider*> CollisionEngine::terrainColliders;
GameObject* CollisionEngine::player;
vector<GameObject*> CollisionEngine::entities;
vector<GameObject*> CollisionEngine::bullets;

void CollisionEngine::Update(const vector<shared_ptr<GameObject>>& objects) {
    ClearState();

    for (auto& obj : objects) {
        auto isoColliders = obj->GetAllComponents(CType::IsoCollider);
        for (auto component : isoColliders) {
            auto isoCollider = (IsoCollider*)component;
            auto& tags = isoCollider->tags;

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
    player = Player::player ? &Player::player->Associated() : nullptr;
    entities.clear();
    bullets.clear();
}

// Requires state to be `Update`d!
void CollisionEngine::Solve() {
    // Player--Terrain collisions
    if (player) {
        Vec2<Cart> pos = player->box.Foot();
        pos.y -= 15;  // !
        for (int i = 0; i < 2; i++) {
            for (auto& collider : terrainColliders) {
                pos = IsoSolver::Solve(*collider, pos);
            }
        }
        pos.y += 15;  // !
        player->box.SetFoot(pos);
    }

    // Enemy--Terrain collisions
    for (auto enemy : entities) {
        Vec2<Cart> pos = enemy->box.Foot();
        pos.y -= 15;  // !
        for (int i = 0; i < 2; i++) {
            for (auto& collider : terrainColliders) {
                pos = IsoSolver::Solve(*collider, pos);
            }
        }
        pos.y += 15;  // !
        enemy->box.SetFoot(pos);
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
