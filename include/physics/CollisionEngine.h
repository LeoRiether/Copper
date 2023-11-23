#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"
#include "component/IsoCollider.h"
#include "component/Player.h"

using std::shared_ptr;
using std::vector;

// Honestly, this is not a class, this is a namespace
class CollisionEngine {
   public:
    static void Update(const vector<shared_ptr<GameObject>>& objects);
    static void Solve();
    static bool TerrainContains(const Vec2<Iso> point);

   private:
    static vector<IsoCollider*> terrainColliders;
    static GameObject* player;
    static vector<GameObject*> entities;

    static void ClearState();
};
