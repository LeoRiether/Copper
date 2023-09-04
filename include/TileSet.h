#pragma once

#include <memory>
#include <string>

#include "Sprite.h"
using std::string;
using std::unique_ptr;

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
