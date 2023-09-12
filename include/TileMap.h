#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "TileSet.h"
using std::string;
using std::vector;
using std::unique_ptr;

class TileMap : public Component {
   private:
    vector<int> tileMatrix;
    unique_ptr<TileSet> tileSet;
    int width, height, depth;

   public:
    const char* DebugName() { return "TileMap"; }

    TileMap(GameObject& associated, const string& file, TileSet* tileSet);

    void Load(const string& file);
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y); // separated for performance reasons
    int& At(int x, int y, int z);
    void Update(float dt);
    void Render();
    void Render(Vec2 cameraPosition);
    void RenderLayer(int layer, int cameraX = 0, int cameraY = 0);
    bool Is(CType type);

    int Width();
    int Height();
    int Depth();
};
