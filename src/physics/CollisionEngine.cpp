#include "physics/CollisionEngine.h"

#include "CType.h"
#include "component/enemy/Enemy.h"
#include "physics/IsoSolver.h"
#include "util.h"

#define MODULE "CollisionEngine"

vector<IsoCollider*> CollisionEngine::terrainColliders;
GameObject* CollisionEngine::player;
vector<GameObject*> CollisionEngine::enemies;

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

        auto enemy = (Enemy*)obj->GetComponent(CType::Enemy);
        if (enemy) {
            enemies.emplace_back(&enemy->associated);
        }
    }

    // Player--Terrain collisions
    if (player) {
        Vec2<Cart> pos = player->box.Foot();
        for (int i = 0; i < 2; i++) {
            for (auto& collider : terrainColliders) {
                pos = IsoSolver::Solve(*collider, pos);
            }
        }
        player->box.SetFoot(pos);
    }

    // Enemy--Terrain collisions
    for (auto enemy : enemies) {
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
    enemies.clear();
}
