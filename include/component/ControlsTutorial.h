#pragma once

#include <functional>
#include <vector>

#include "CType.h"
#include "Component.h"
#include "GameObject.h"
#include "InputManager.h"
#include "component/Text.h"

using std::function;
using std::vector;

class ControlsTutorial : public Component {
   public:
    ControlsTutorial(GameObject& go);

    void Update(float dt);

    inline CType Key() const { return CType::ControlsTutorial; }

   private:
    struct State {
        const char* text;
        function<bool(InputManager&)> shouldAdvance;
    };

    int index{0};
    constexpr static float fadingTime{0.5};
    float fadingIn{0}, fadingOut{0}, alpha{255};
    static vector<State> states;
};
