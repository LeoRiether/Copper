#include "physics/CollisionEngine.h"

#include "CType.h"
#include "component/IsoCollider.h"
#include "component/Player.h"
#include "physics/IsoSolver.h"
#include "util.h"

#define MODULE "CollisionEngine"

void CollisionEngine::Solve(const vector<shared_ptr<GameObject>>& objects) {
    static vector<IsoCollider*> terrainColliders;
    terrainColliders.clear();

    static GameObject* player;
    player = Player::player ? &Player::player->Associated() : nullptr;

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
        for (auto& collider : terrainColliders) {
            pos = IsoSolver::Solve(*collider, pos);
        }
        player->box.SetFoot(pos);
    }
}
