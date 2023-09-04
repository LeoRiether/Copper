#include "TileMap.h"

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

void TileMap::Render() {
    // TODO: what?
    RenderLayer(0);
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
    // TODO: start y at the point where rendery >= 0
    const Rect& viewport = associated.box;
    for (int y = 0; y < height; y++) {
        const float rendery = y * tileSet->TileHeight() - cameraY;
        if (rendery > viewport.h) break;

        for (int x = 0; x < width; x++) {
            const float renderx = x * tileSet->TileWidth() - cameraX;
            if (renderx > viewport.w) break;

            tileSet->RenderTile(At(x, y, layer), renderx, rendery);
        }
    }
}

bool TileMap::Is(CType type) { return type == CType::TileMap; }

int TileMap::Width() { return width; }
int TileMap::Height() { return height; }
int TileMap::Depth() { return depth; }
