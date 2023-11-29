#pragma once

#include <string>

#include "Component.h"
#include "GameObject.h"
#include "component/Sprite.h"

using std::string;
using std::unique_ptr;

class Tileset : public Component {
   public:
    unique_ptr<Sprite> sprite;
    // These refer to the size *inside the sprite*, not scaled!
    int TileWidth{0}, TileHeight{0};

    Tileset(GameObject& go, const string& filename, int columns, int rows);

    inline bool Is(CType type) { return type == CType::Tileset; }

    struct Coords {
        int row;
        int col;
    };
    inline Coords At(int id) { return {id / columns, id % columns}; }
    inline int At(int i, int j) { return i * columns + j; }
    inline int At(Coords c) { return At(c.row, c.col); }

    /** ClipTo sets the underlying Sprite's clipRect so the tile with the given
     * id is rendered */
    void ClipTo(int id);
    void ClipTo(int row, int col);
    void ClipTo(Coords c);

    Tileset* WithScale(float s);

   private:
    int columns, rows;
};
