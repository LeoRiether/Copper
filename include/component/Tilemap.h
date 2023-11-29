#pragma once

#include <vector>

#include "Component.h"

using std::string;
using std::vector;

class Tilemap : public Component {
   public:
    struct Coord {
        int i, j;
    };
    vector<Coord> Components;

    Tilemap(GameObject& go, const string& csv);

    void Render(Vec2<Cart> camera);

    inline CType Key() const { return CType::Tilemap; }
    inline int At(int i, int j) { return i * width + j; }

    Tilemap* WithOffset(Vec2<Cart> c);

    /** Adds IsoColliders to every single non-negative tile */
    Tilemap* WithColliders(Rect base);

    /** Computes the connected components of the tilemap */
    void ComputeComponents();

   private:
    int width{0}, height{0};
    vector<int> map;
    Coord offset;

    void load(const string& csv);
};
