#pragma once

#include <vector>

#include "Component.h"

using std::string;
using std::vector;

class Tilemap : public Component {
   public:
    Tilemap(GameObject& go, const string& csv);

    void Render(Vec2<Cart> camera);

    struct Coord {
        int i, j;
    };
    inline bool Is(CType type) { return type == CType::Tilemap; };
    inline int At(int i, int j) { return i * width + j; }

    Tilemap* WithOffset(Vec2<Cart> c);

    /** Computes the connected components of the tilemap */
    void ComputeComponents();
    vector<Coord> Components;

   private:
    int width{0}, height{0};
    vector<int> map;
    Coord offset;

    void load(const string& csv);
};
