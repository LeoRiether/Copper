#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "GameObject.h"
#include "TileSet.h"
using std::string;
using std::unique_ptr;
using std::vector;

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
    int& At(int x, int y);  // separated for performance reasons
    int& At(int x, int y, int z);
    void Update(float dt);
    void Render();
    void Render(Vec2<Cart> cameraPosition);
    void RenderLayer(int layer, int cameraX = 0, int cameraY = 0);
    bool Is(CType type);

    int Columns();
    int Rows();
    int Depth();

    int Width();   // in pixels!
    int Height();  // in pixels!
};
