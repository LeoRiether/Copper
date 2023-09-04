#pragma once

#include <string>
#include <vector>

#include "Component.h"
#include "GameObject.h"
#include "TileSet.h"
using std::string;
using std::vector;

class TileMap : Component {
   private:
    vector<int> tileMatrix;
    TileSet* tileSet;
    int width, height, depth;

   public:
    TileMap(GameObject& associated, const string& file, TileSet* tileSet);

    void Load(const string& file);
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y); // separated for performance reasons
    int& At(int x, int y, int z);
    void Render();
    void RenderLayer(int layer, int cameraX = 0, int cameraY = 0);

    int Width();
    int Height();
    int Depth();
};
