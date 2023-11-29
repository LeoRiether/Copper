#include "component/Tileset.h"

#include "Component.h"
#include "GameObject.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "Tileset"

Tileset::Tileset(GameObject& go, const string& filename, int columns, int rows)
    : Component(go), columns(columns), rows(rows) {
    sprite = unique_ptr<Sprite>(new Sprite{go, filename});

    if (sprite->SheetWidth() % columns != 0) {
        warn2(
            "SheetWidth (%d) is not divisible by columns (%d), maybe you "
            "counted the columns wrong?",
            sprite->SheetWidth(), columns);
    }
    if (sprite->SheetHeight() % rows != 0) {
        warn2(
            "SheetHeight (%d) is not divisible by rows (%d), maybe you counted "
            "the rows wrong?",
            sprite->SheetHeight(), rows);
    }

    TileWidth = sprite->SheetWidth() / columns;
    TileHeight = sprite->SheetHeight() / rows;
    sprite->SetClip(0, 0, TileWidth, TileHeight);
}

void Tileset::ClipTo(int row, int col) {
    sprite->clipRect.x = col * TileWidth;
    sprite->clipRect.y = row * TileHeight;
}

void Tileset::ClipTo(Coords c) { ClipTo(c.row, c.col); }

void Tileset::ClipTo(int id) { return ClipTo(At(id)); }

Tileset* Tileset::WithScale(float s) {
    sprite->SetScale(s);
    return this;
}
