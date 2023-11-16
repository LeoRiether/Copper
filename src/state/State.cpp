#include "state/State.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <tuple>

#include "CType.h"
#include "GameObject.h"
#include "component/IsoCollider.h"
#include "util.h"

#define MODULE "State"

State::State() { camera->SetSpeed(Vec2<Cart>{250, 250}); }

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
    ZSort();
    for (auto& go : objects) {
        go->Render(camera->Pos());
    }
}

// TODO: separate cartesians and isos, sort them separately, then merge
void State::ZSort() {
    auto pointLessThanIso = [&](const Vec2<Cart> point,
                                const IsoCollider* iso) {
        auto pointIso = point.toIso();
        auto lineFrom = iso->box.BotLeft().transmute<Iso>();
        auto lineTo = iso->box.TopRight().transmute<Iso>();

        auto P = lineTo - lineFrom;
        auto Q = pointIso - lineFrom;

        auto cross = P.x * Q.y - P.y * Q.x;
        return cross < 0;
    };

    // Sort by render layer
    std::stable_sort(objects.begin(), objects.end(), [&](auto& a, auto& b) {
        return a->renderLayer < b->renderLayer;
    });

    // For each layer, separate cartesian objects and isocollider objects,
    // sort them separately, then merge them back into `objects`
    int n = objects.size();
    for (int l = 0; l < n;) {
        static vector<shared_ptr<GameObject>> cartesians, isos;
        cartesians.clear();
        isos.clear();

        // We'll process a range in which every object has the same renderLayer
        int r = l;
        while (r < n && objects[r]->renderLayer == objects[l]->renderLayer) {
            auto isoA =
                (IsoCollider*)objects[r]->GetComponent(CType::IsoCollider);

            if (isoA)
                isos.emplace_back(objects[r]);
            else
                cartesians.emplace_back(objects[r]);

            r++;
        }

        std::stable_sort(cartesians.begin(), cartesians.end(),
                         [&](auto& a, auto& b) {
                             return a->box.Foot().y < b->box.Foot().y;
                         });

        std::stable_sort(isos.begin(), isos.end(), [&](auto& a, auto& b) {
            auto isoA = (IsoCollider*)a->GetComponent(CType::IsoCollider);
            auto isoB = (IsoCollider*)b->GetComponent(CType::IsoCollider);
            return isoA->box.Center().toCart().y <
                   isoB->box.Center().toCart().y;
        });

        // Merge cartesians and isos
        int nc = cartesians.size(), ni = isos.size();
        int ic = 0, ii = 0;
        while (ic < nc && ii < ni) {
            auto iso = (IsoCollider*)isos[ii]->GetComponent(CType::IsoCollider);
            auto cart = cartesians[ic]->box.Foot();
            objects[l++] =
                pointLessThanIso(cart, iso) ? cartesians[ic++] : isos[ii++];
        }
        while (ic < nc) objects[l++] = cartesians[ic++];
        while (ii < ni) objects[l++] = isos[ii++];
    }
}
