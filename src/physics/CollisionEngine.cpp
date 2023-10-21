#include "physics/CollisionEngine.h"

#include "CType.h"
#include "physics/IsoSolver.h"
#include "util.h"

#define MODULE "CollisionEngine"

vector<IsoCollider*> CollisionEngine::terrainColliders;
GameObject* CollisionEngine::player;

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
}

void CollisionEngine::ClearState() {
    terrainColliders.clear();
    player = Player::player ? &Player::player->Associated() : nullptr;
}
