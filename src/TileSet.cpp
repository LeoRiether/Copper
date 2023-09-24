#include "TileSet.h"

#include "util.h"

#define MODULE "TileSet"

TileSet::TileSet(int tileWidth, int tileHeight, const string& file)
    : go(new GameObject{}),
      tileSet(new Sprite(*go, file)),
      tileWidth(tileWidth),
      tileHeight(tileHeight) {
    rows = tileSet->SheetHeight() / tileHeight;
    columns = tileSet->SheetWidth() / tileWidth;

    if (tileSet->SheetHeight() % tileHeight != 0) {
        warn2(
            "tileSet->Height (%d) is not divisible by tileHeight (%d) in " BLUE
            "%s" RESET,
            tileSet->SheetHeight(), tileHeight, file.c_str());
    }
    if (tileSet->SheetWidth() % tileWidth != 0) {
        warn2("tileSet->Width (%d) is not divisible by tileWidth (%d) in " BLUE
              "%s" RESET,
              tileSet->SheetWidth(), tileWidth, file.c_str());
    }
}

void TileSet::RenderTile(int index, float x, float y) {
    if (index == -1) return;  // with no warning
    if (index < 0 || (uint64_t)index >= (uint64_t)rows * columns) {
        warn2("index out of bounds (0 <= %d <= %lu)", index,
              (uint64_t)rows * columns);
        return;
    }

    tileSet->SetClip((index % columns) * tileWidth,
                     (index / columns) * tileHeight, tileWidth, tileHeight);
    tileSet->RenderAt(x, y);
}

int TileSet::TileWidth() { return tileWidth; }
int TileSet::TileHeight() { return tileHeight; }
