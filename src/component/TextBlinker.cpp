#include "component/TextBlinker.h"

#include "math/Interpolate.h"
#include "component/Text.h"

TextBlinker::TextBlinker(GameObject& associated, float fullBlinkTimeS)
    : Component(associated), fullBlinkTimeS(fullBlinkTimeS) {}

void TextBlinker::Update(float dt) {
    currentTime += dt;
    while (currentTime >= fullBlinkTimeS) {
        currentTime -= fullBlinkTimeS;
    }

    // ...quadratic interpolation?
    auto qerp = [&](float from, float to, float p) {
        return lerp(from, to, (1 - p) * (p - 1) + 1);
    };

    float halfTime = fullBlinkTimeS / 2.0f;
    float alpha;
    if (currentTime <= halfTime)
        alpha = qerp(0.0f, 255.0f, currentTime / halfTime);
    else
        alpha = qerp(0.0f, 255.0f, 1.0f - (currentTime - halfTime) / halfTime);

    auto text = (Text*)associated.GetComponent(CType::Text);
    auto color = text->Color();
    color.a = alpha;
    text->SetColor(color);
}

void TextBlinker::Render(Vec2<Cart>) {}
