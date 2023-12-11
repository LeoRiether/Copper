#include "state/State.h"

#include <algorithm>
#include <memory>

#include "CType.h"
#include "Game.h"
#include "GameObject.h"
#include "component/IsoCollider.h"
#include "util.h"

#define MODULE "State"

State::State() {
	camera->SetSpeed(Vec2<Cart>{250, 250});
}

State::~State() {
    objects.clear();
    addRequests.clear();
}

void State::LoadAssets() {}
void State::Start() { started = true; }
void State::Pause() {}
void State::Resume() {}

weak_ptr<GameObject> State::RequestAddObject(GameObject* go) {
    shared_ptr<GameObject> ptr{go};
    ptr->weak = std::weak_ptr(ptr);
    addRequests.emplace_back(ptr);
    return ptr;
}

// WARN: partial function, only call if you're sure the GameObject is in
// `objects`
weak_ptr<GameObject> State::GetObject(GameObject* go) {
    for (auto& candidate : objects) {
        if (go == candidate.get()) return candidate;
    }
    fail("GetObject called with GameObject that's not registered");
}

void State::ProcessAddRequests() {
    while (!addRequests.empty()) {
        auto newObjects = std::move(addRequests);
        addRequests = {};
        for (auto& go : newObjects) {
            objects.emplace_back(go);
        }
        for (auto& go : newObjects) {
            go->Start();
        }
    }
}

void State::StartArray() {
    for (auto& go : objects) {
        go->Start();
    }
    ProcessAddRequests();
}

void State::UpdateArray(float dt) {
    for (auto& go : objects) {
        go->Update(dt);
    }
}

void State::RenderArray() {
    auto so = ZSort();
    for (auto& s : so) {
        switch (s.tag) {
            case SortableObject::Object: {
                s.go->Render(camera->Pos());
                break;
            }
            case SortableObject::Tile: {
                auto tilemap = (Tilemap*)s.go->GetComponent(CType::Tilemap);
                if (!tilemap) fail("rendering tile without tilemap?");
                auto camera = Game::Instance().GetState().GetCamera().Pos();
                tilemap->RenderTile(camera, s.tile.index);
                break;
            }
        }
    }
}

vector<State::SortableObject> State::ZSort() {
    static vector<SortableObject> so;
    so.clear();
    so.reserve(objects.size());

    auto camera = Game::Instance().GetState().GetCamera().Pos();
    for (auto& go : objects) {
        auto tilemap = (Tilemap*)go->GetComponent(CType::Tilemap);
        if (tilemap && tilemap->base.w != 0 && tilemap->base.h != 0) {
            auto tiles = tilemap->RenderedTiles(camera);
            so.reserve(so.size() + tiles.size());
            for (auto tile : tiles) {
                so.push_back({SortableObject::Tile, go.get(), tile});
            }
        } else {
            so.push_back({SortableObject::Object, go.get(),
                          Tilemap::TileToRender{0, Rect{0}}});
        }
    }

    auto key = [&](const SortableObject& x) {
        // Tile key
        if (x.tag == SortableObject::Tile) {
            auto center = x.tile.collider.Center().transmute<Iso>().toCart();
            return std::tuple{x.go->renderLayer, center.y};
        }

        // Iso object key
        auto iso = (IsoCollider*)x.go->GetComponent(CType::IsoCollider);
        if (iso) {
            auto center = iso->box.Center().transmute<Iso>().toCart();
            return std::tuple{x.go->renderLayer, center.y};
        }

        // Anything else
        return std::tuple{x.go->renderLayer, x.go->box.Foot().y};
    };

    std::stable_sort(so.begin(), so.end(), [&](const auto& a, const auto& b) {
        return key(a) < key(b);
    });

    return so;
}
