#pragma once

#include <optional>
#include <string>

#include "GameObject.h"
#include "State.h"
#include "math/Vec2.h"

using std::optional;
using std::string;

class ViewerState : public State {
   public:
    void LoadAssets();
    void Start();
    void Pause();
    void Resume();

    void Update(float dt);
    void Render();

   private:
    float zoom{1};
    Vec2<Cart> offset{0, 0};
    string mode{"iso"};
    weak_ptr<GameObject> image{};
    Vec2<Cart> lastMousePos{0, 0};

    struct MouseDrag {
        Vec2<Cart> from, to;
    };
    optional<MouseDrag> drag;
    Rect dragRect;
};
