#include "component/FPSCounter.h"

#include <string>

#include "Colors.h"
#include "util.h"

#define MODULE "FPSCounter"

FPSCounter::FPSCounter(GameObject& go) : Component(go) {
    text = new Text{go,  ASSETS "/font/AldotheApache.ttf",
                    30,  Text::Blended,
                    "?", colorFromHex("f4c36e")};
    associated.renderLayer = 50;
}

void FPSCounter::Update(float dt) {
    sum -= dts[i];
    dts[i] = 1.0 / dt;
    sum += dts[i];

    i++;
    if (i >= N) i = 0;

    string s = std::to_string(sum / N);
    text->SetText(s);
    associated.box.SetTopLeft({10, 10});
}

void FPSCounter::Render(Vec2<Cart>) { text->Render({0, 0}); }
