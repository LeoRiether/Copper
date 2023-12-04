#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "GameObject.h"
#include "component/Collider.h"
#include "component/IsoCollider.h"

using std::shared_ptr;
using std::unordered_map;
using std::vector;

using chunk_key = int64_t;
struct chunk2 {
    int i, j;
};

// Honestly, this is not a class, this is a namespace
class CollisionEngine {
   public:
    static void Update(const vector<shared_ptr<GameObject>>& objects);
    static void Solve();
    static bool TerrainContains(const Vec2<Iso> point);
    static bool TerrainContainsSegment(const Vec2<Iso> A, const Vec2<Iso> B);

   private:
    constexpr static int ChunkSize = 256;

    struct VT {
        Collider* c;
        GameObject* go;
    };
    struct IG {
        IsoCollider* c;
        GameObject* go;
    };

    static vector<IsoCollider*> terrainColliders;
    static unordered_map<chunk_key, vector<IsoCollider*>> cTerrainColliders;
    static vector<VT> vterrainColliders;
    static vector<IG> triggers;
    static GameObject* player;
    static vector<GameObject*> entities;
    static vector<GameObject*> bullets;

    static void ClearState();

    static inline chunk_key ChunkKey(chunk2);
    static inline chunk2 Chunk2(Vec2<Cart>);
    static inline chunk2 IsoColliderChunk(IsoCollider*);
};
