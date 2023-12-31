#include "component/Tilemap.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_set>

#include "Consts.h"
#include "Game.h"
#include "component/IsoCollider.h"
#include "component/Tileset.h"
#include "util.h"

#define MODULE "Tilemap"

Tilemap::Tilemap(GameObject& go, const string& csv) : Component(go) {
    load(csv);
}

void Tilemap::Start() {
    tileset = (Tileset*)associated.GetComponent(CType::Tileset);
    if (!tileset) {
        warn("Tilemap does not have an associated tileset!");
        return;
    }

    tileWidth = tileset->TileWidth * tileset->sprite->Scale().x;
    tileHeight = tileset->TileHeight * tileset->sprite->Scale().y;
}

/////////////////////////////
//        Rendering        //
/////////////////////////////
void Tilemap::Render(Vec2<Cart> camera) {
    // ~Culling~
    const Vec2<Cart> screenCenter{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    const Vec2<Iso> P =
        (screenCenter + camera - associated.box.TopLeft()).toIso();
    int mid_j = P.x / tileWidth - offset.j;
    int mid_i = P.y * 2.0f / tileHeight - offset.i;

    constexpr int less = 15;
    constexpr int more = 11;
    // constexpr int less = 150;
    // constexpr int more = 110;

    for (int i = std::max(0, mid_i - less); i < mid_i + more && i < height; i++) {
        for (int j = std::max(0, mid_j - less); j < mid_j + more && j < width;
             j++) {
            int id = map[At(i, j)];
            if (id < 0) continue;

            auto pos = worldPosition(i, j).toCart() - camera +
                       associated.box.TopLeft();
            tileset->ClipTo(id);
            tileset->sprite->RenderAt(pos.x, pos.y);
        }
    }
}

vector<Tilemap::TileToRender> Tilemap::RenderedTiles(Vec2<Cart> camera) {
    // Copy-pasted from `Tilemap::Render`, sorry

    // ~Culling~
    const Vec2<Cart> screenCenter{SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    const Vec2<Iso> P =
        (screenCenter + camera - associated.box.TopLeft()).toIso();
    int mid_j = P.x / tileWidth - offset.j;
    int mid_i = P.y * 2.0f / tileHeight - offset.i;

    vector<TileToRender> result;
    for (int i = std::max(0, mid_i - 14); i < mid_i + 11 && i < height; i++) {
        for (int j = std::max(0, mid_j - 15); j < mid_j + 10 && j < width;
             j++) {
            int id = map[At(i, j)];
            if (id < 0) continue;

            auto b = baseForTile(i, j);
            b.OffsetBy(associated.box.TopLeft().toIso().transmute<Cart>());
            result.push_back({At(i, j), b});
        }
    }
    return result;
}

void Tilemap::RenderTile(Vec2<Cart> camera, int id) {
    auto [i, j] = At(id);
    auto pos = worldPosition(i, j).toCart() - camera + associated.box.TopLeft();
    tileset->ClipTo(map[id]);
    tileset->sprite->RenderAt(pos.x, pos.y);

    static int& showColliders = Consts::GetInt("debug.show_colliders");
    if (showColliders) {
        auto c = IsoCollider{associated};
        c.WithBase(baseForTile(i, j));
        c.Update(0);
        c.Render(camera);
    }
}

Vec2<Iso> Tilemap::worldPosition(int i, int j) {
    return Vec2<Iso>{(j + offset.j) * tileWidth,
                     (i + offset.i) * tileHeight / 2.0f};
}

Rect Tilemap::baseForTile(int i, int j) {
    auto& sprite = tileset->sprite;
    auto b = base;
    b.x *= sprite->Scale().x;
    b.y *= sprite->Scale().y;
    b.w *= sprite->Scale().x;
    b.h *= sprite->Scale().y;
    b.OffsetBy(worldPosition(i, j).toIso().transmute<Cart>());
    return b;
}

////////////////////////////////
//        Builder/With        //
////////////////////////////////
Tilemap* Tilemap::WithOffset(Vec2<Cart> c) {
    offset.i = c.x;
    offset.j = c.y;
    return this;
}

// This really really looks like the Render function
Tilemap* Tilemap::WithColliders(Rect base) {
    Start();  // just to make sure we have everything.....
    this->base = base;

    std::unordered_set<int> ignoredTileIds = {173, 185};

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int id = map[At(i, j)];
            if (id < 0 || ignoredTileIds.count(id)) continue;

            auto b = baseForTile(i, j);
            associated.AddComponent((new IsoCollider{associated})
                                        ->WithTag(tag::Terrain)
                                        ->WithBase(b));
        }
    }

    return this;
}

Tilemap* Tilemap::WithFloorColliders(Rect base) {
    Start();  // just to make sure we have everything.....
    this->base = base;

    const std::pair<int, int> delta[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = -1; i <= height; i++) {
        for (int j = -1; j <= width; j++) {
            auto filled = [&](int i, int j) {
                return i >= 0 && i < height && j >= 0 && j < width &&
                       map[At(i, j)] >= 0;
            };
            if (filled(i, j)) continue;

            bool isBorder = false;
            for (int k = 0; k < 4; k++) {
                auto [di, dj] = delta[k];
                isBorder |= filled(i + di, j + dj);
            }
            if (!isBorder) continue;

            auto b = baseForTile(i, j);
            associated.AddComponent((new IsoCollider{associated})
                                        ->WithTag(tag::Terrain)
                                        ->WithBase(b));
        }
    }

    this->base = {0, 0, 0, 0};
    return this;
}

////////////////////////////////////////
//        Connected components        //
////////////////////////////////////////
void Tilemap::ComputeComponents() {
    vector<vector<bool>> visited(height, vector<bool>(width));
    vector<std::pair<int, int>> deltas{{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    auto dfs = [&](int startI, int startJ) {
        std::stack<std::pair<int, int>> nodes;
        visited[startI][startJ] = true;
        nodes.emplace(startI, startJ);

        long long sumI = 0, sumJ = 0;
        int totalNodes = 0;

        while (!nodes.empty()) {
            auto [i, j] = nodes.top();
            nodes.pop();

            totalNodes++;
            sumI += i;
            sumJ += j;

            for (auto delta : deltas) {
                int ni = i + delta.first;
                int nj = j + delta.second;
                if (ni < 0 || ni >= height || nj < 0 || nj >= width ||
                    visited[ni][nj] || map[At(ni, nj)] < 0)
                    continue;

                visited[ni][nj] = true;
                nodes.emplace(ni, nj);
            }
        }

        if (totalNodes > 10)
            Components.push_back(
                Coord{int(sumI / totalNodes), int(sumJ / totalNodes)});
    };

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!visited[i][j] && map[At(i, j)] >= 0) dfs(i, j);
        }
    }
}

//////////////////////////////
//        Map loader        //
//////////////////////////////
void Tilemap::load(const string& csv) {
    std::ifstream file;
    file.open(csv);
    if (!file.is_open()) {
        fail2("Failed to open %s", csv.c_str());
    }

    // vector<int> tempMap;
    string line;
    while (getline(file, line)) {
        std::stringstream tokens{line};
        int id;
        while (tokens >> id) {
            if (height == 0) width++;
            map.push_back(id);
            tokens.ignore(1, ',');
        }
        if (width > 0) height++;
    }
    log2("map is %d x %d", height, width);

    // ~~Copy map transmuted.....~~
    // omg I didn't need to transmute it
    // map.resize(tempMap.size());
    // std::swap(width, height);
    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         // I have no idea what I'm doing
    //         map[i * width + j] = tempMap[j * height + i];
    //     }
    // }

    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         if (map[At(i, j)] > 0)
    //             std::cout << std::setw(3) << map[At(i, j)] << ' ';
    //         std::cout << "    ";
    //     }
    //     std::cout << std::endl;
    // }

    file.close();
}
