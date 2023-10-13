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
    string mode{"iso"};
    weak_ptr<GameObject> image{};

    struct MouseDrag {
        Vec2<Cart> from, to;
    };
    optional<MouseDrag> drag;
    Rect dragRect;
};
