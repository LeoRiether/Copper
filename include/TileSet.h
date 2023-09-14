#pragma once

#include <memory>
#include <string>

#include "component/Sprite.h"
using std::string;
using std::unique_ptr;

constexpr int DEFAULT_TILE_WIDTH = 64;
constexpr int DEFAULT_TILE_HEIGHT = 64;

class TileSet {
   private:
    unique_ptr<GameObject> go;
    Sprite* tileSet;
    int rows, columns, tileWidth, tileHeight;

   public:
    TileSet(int tileWidth, int tileHeight, const string& file);

    void RenderTile(int index, float x, float y);

    int TileWidth();
    int TileHeight();
};
