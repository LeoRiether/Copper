#include "component/ControlsTutorial.h"

#include "Game.h"
#include "InputManager.h"
#include "component/Player.h"
#include "component/Text.h"
#include "util.h"

#define MODULE "ControlsTutorial"

vector<ControlsTutorial::State> ControlsTutorial::states = {
    {
        "This state advances after the first frame",
        [](InputManager&) { return true; },
    },
    {
        "press W A S D\nto move",
        [](InputManager& input) {
            return input.IsKeyDown(MOVE_UP_KEY) ||
                   input.IsKeyDown(MOVE_DOWN_KEY) ||
                   input.IsKeyDown(MOVE_LEFT_KEY) ||
                   input.IsKeyDown(MOVE_RIGHT_KEY);
        },
    },
    {
        "press SPACE\nto dash",
        [](InputManager& input) { return input.KeyPress(DASH_KEY); },
    },
    {
        "LEFT CLICK with\nyour mouse\nto fire",
        [](InputManager& input) { return input.IsMouseDown(1); },
    },
    {
        "KILL all enemies\nto win\n Good Luck",
        [](InputManager&) {
            static float hackyTimer{1.2};
            hackyTimer -= FRAME_MS / 1000.0f;
            return hackyTimer <= 0;
        },
    }};

ControlsTutorial::ControlsTutorial(GameObject& go) : Component(go) {}

// Convoluted logic, but I don't feel like converting this to a state machine
void ControlsTutorial::Update(float dt) {
    auto text = (Text*)associated.GetComponent(CType::Text);
    if (!text) fail("no associated Text");

    auto player = Player::player;
    if (player) {
        associated.box.SetFoot(player->associated.box.Head());
    }

    auto lerp = [&](float p, float from, float to) {
        return from + (to - from) * p;
    };

    if (fadingOut > 0) {
        setAlpha(*text, lerp((fadingTime - fadingOut) / fadingTime, 255, 0));
        fadingOut -= dt;
        if (fadingOut <= 0) {
            index++;
            if (index < (int)states.size()) {
                text->SetText(states[index].text);
                fadingIn = fadingTime;  // switchState(FadingIn)
            } else {
                associated.RequestDelete();  // :knife:
            }
        }
    } else if (fadingIn > 0) {
        setAlpha(*text, lerp((fadingTime - fadingIn) / fadingTime, 0, 255));
        fadingIn -= dt;
        if (fadingIn <= 0) {
            setAlpha(*text, 255);
            // switchState(WaitForInput)
        }
    } else {
        auto& input = InputManager::Instance();
        if (states[index].shouldAdvance(input)) {
            fadingOut = fadingTime;  // switchState(FadeOut)
        }
    }
}

void ControlsTutorial::setAlpha(Text& text, float a) {
    alpha = a;
    auto color = text.Color();
    color.a = alpha;
    text.SetColor(color);
}
