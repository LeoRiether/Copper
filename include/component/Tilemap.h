#pragma once

#include <vector>

#include "Component.h"
#include "component/Tileset.h"
#include "math/Vec2.h"

using std::string;
using std::vector;

class Tilemap : public Component {
   public:
    struct Coord {
        int i, j;
    };
    struct TileToRender {
        int index;
        Rect collider;
    };

    vector<Coord> Components;
    Rect base{0, 0, 0, 0};

    Tilemap(GameObject& go, const string& csv);

    void Start();

    void Render(Vec2<Cart> camera);
    vector<TileToRender> RenderedTiles(Vec2<Cart> camera);
    void RenderTile(Vec2<Cart> camera, int index);

    inline CType Key() const { return CType::Tilemap; }
    inline int At(int i, int j) { return i * width + j; }
    inline Coord At(int id) { return {id / width, id % width}; }

    Tilemap* WithOffset(Vec2<Cart> c);

    /** Adds IsoColliders to every single non-negative tile */
    Tilemap* WithColliders(Rect base);

    /** Computes the connected components of the tilemap */
    void ComputeComponents();

   private:
    int width{0}, height{0};
    vector<int> map;
    Coord offset;

    Tileset* tileset{nullptr};
    float tileWidth{0}, tileHeight{0};

    Vec2<Iso> worldPosition(int i, int j);
    Rect baseForTile(int i, int j);

    void load(const string& csv);
};
