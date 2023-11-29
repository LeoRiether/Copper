#include "component/Tilemap.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>

#include "component/IsoCollider.h"
#include "component/Tileset.h"
#include "util.h"

#define MODULE "Tilemap"

Tilemap::Tilemap(GameObject& go, const string& csv) : Component(go) {
    load(csv);
}

void Tilemap::Render(Vec2<Cart> camera) {
    auto tileset = (Tileset*)associated.GetComponent(CType::Tileset);
    if (!tileset) {
        warn("Tilemap does not have an associated tileset!");
        return;
    }

    const float tileWidth = tileset->TileWidth * tileset->sprite->Scale().x;
    const float tileHeight = tileset->TileHeight * tileset->sprite->Scale().y;

    //  PERF: don't render tiles out of sight
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int id = map[At(i, j)];
            if (id < 0) continue;

            Vec2<Cart> pos = Vec2<Iso>{(j + offset.j) * tileWidth,
                                       (i + offset.i) * tileHeight / 2.0f}
                                 .toCart();
            pos = pos - camera + associated.box.TopLeft();
            tileset->ClipTo(id);
            tileset->sprite->RenderAt(pos.x, pos.y);
        }
    }
}

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

Tilemap* Tilemap::WithOffset(Vec2<Cart> c) {
    offset.i = c.x;
    offset.j = c.y;
    return this;
}

// This really really looks like the Render function
Tilemap* Tilemap::WithColliders(Rect base) {
    auto tileset = (Tileset*)associated.GetComponent(CType::Tileset);
    if (!tileset) {
        warn("Tilemap does not have an associated tileset!");
        return this;
    }

    auto& sprite = tileset->sprite;

    const float tileWidth = tileset->TileWidth * tileset->sprite->Scale().x;
    const float tileHeight = tileset->TileHeight * tileset->sprite->Scale().y;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int id = map[At(i, j)];
            if (id < 0) continue;

            Vec2<Iso> pos = {(j + offset.j) * tileWidth,
                             (i + offset.i) * tileHeight / 2.0f};

            auto b = base;
            b.x *= sprite->Scale().x;
            b.y *= sprite->Scale().y;
            b.w *= sprite->Scale().x;
            b.h *= sprite->Scale().y;
            b.OffsetBy(pos.toIso().transmute<Cart>());
            associated.AddComponent((new IsoCollider{associated})
                                        ->WithTag(tag::Terrain)
                                        ->WithBase(b));
        }
    }

    return this;
}

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
