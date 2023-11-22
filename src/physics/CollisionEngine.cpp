#include "physics/CollisionEngine.h"

#include "CType.h"
#include "component/enemy/RobotCan.h"
#include "physics/IsoSolver.h"
#include "util.h"

#define MODULE "CollisionEngine"

vector<IsoCollider*> CollisionEngine::terrainColliders;
GameObject* CollisionEngine::player;
vector<GameObject*> CollisionEngine::entities;

void CollisionEngine::Solve(const vector<shared_ptr<GameObject>>& objects) {
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

        if (obj->tags.test(tag::Entity)) {
            entities.emplace_back(obj.get());
        }
    }

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
        for (int i = 0; i < 2; i++) {
            for (auto& collider : terrainColliders) {
                pos = IsoSolver::Solve(*collider, pos);
            }
        }
        enemy->box.SetFoot(pos);
    }
}

void CollisionEngine::ClearState() {
    terrainColliders.clear();
    player = Player::player ? &Player::player->Associated() : nullptr;
    entities.clear();
}
