#include "component/TileMap.h"

#include <fstream>

#include "Game.h"
#include "util.h"

#define MODULE "TileMap"

TileMap::TileMap(GameObject& associated, const string& file, TileSet* tileSet)
    : Component(associated), tileSet(tileSet) {
    Load(file);
}

void TileMap::Load(const string& file) {
    std::ifstream stream(file);
    if (!stream.is_open())
        fail2("couldn't open " BLUE "%s" RESET, file.c_str());

    char comma;
    stream >> width >> comma >> height >> comma >> depth >> comma;

    tileMatrix.resize(width * height * depth);

    size_t index = 0;  // easier indexing :D
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                stream >> tileMatrix[index] >> comma;
                tileMatrix[index++]--;  // ty C++
            }
        }
    }
}

void TileMap::SetTileSet(TileSet* ts) { tileSet = unique_ptr<TileSet>(ts); }

int& TileMap::At(int x, int y) { return tileMatrix[x + y * width]; }
int& TileMap::At(int x, int y, int z) {
    return tileMatrix[x + y * width + z * width * height];
}

void TileMap::Update(float dt) { UNUSED(dt); }

void TileMap::Render() { Render(Vec2{0, 0}); }

void TileMap::Render(Vec2 cameraPosition) {
    for (int z = 0; z < depth; z++) {
        RenderLayer(z, cameraPosition.x * (z * 0.8f + 1),
                    cameraPosition.y * (z * 0.8f + 1));
    }
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
    const Rect& viewport = associated.box;
    const int tileHeight = tileSet->TileHeight();
    const int tileWidth = tileSet->TileWidth();
    const int starty = std::max(0, cameraY / tileHeight);
    const int startx = std::max(0, cameraX / tileWidth);
    const int endy = std::min<int>(
        height, (cameraY + viewport.h + tileHeight - 1) / tileHeight);
    const int endx = std::min<int>(
        width, (cameraX + viewport.w + tileWidth - 1) / tileWidth);

    for (int y = starty; y < endy; y++) {
        const float rendery = y * tileHeight - cameraY;
        for (int x = startx; x < endx; x++) {
            const float renderx = x * tileWidth - cameraX;
            tileSet->RenderTile(At(x, y, layer), renderx, rendery);
        }
    }
}

bool TileMap::Is(CType type) { return type == CType::TileMap; }

int TileMap::Columns() { return width; }
int TileMap::Rows() { return height; }
int TileMap::Depth() { return depth; }
int TileMap::Width() { return (width - 1) * tileSet->TileWidth(); }
int TileMap::Height() { return (height - 1) * tileSet->TileHeight(); }
