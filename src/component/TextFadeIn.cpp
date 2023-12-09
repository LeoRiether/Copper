#include "component/TextFadeIn.h"

#define MODULE "TextFadeIn"

#include "component/Text.h"
#include "math/Interpolate.h"

TextFadeIn::TextFadeIn(GameObject& associated, float fullFadeTimeS)
    : Component(associated), fullFadeTimeS(fullFadeTimeS) {}

void TextFadeIn::Update(float dt) {
    currentTime += dt;

    // ...quadratic interpolation?
    auto qerp = [&](float from, float to, float p) {
        return lerp(from, to, (1 - p) * (p - 1) + 1);
    };

    float alpha = currentTime >= fullFadeTimeS
                      ? 255.0f
                      : qerp(0.0f, 255.0f, currentTime / fullFadeTimeS);

    auto text = (Text*)associated.GetComponent(CType::Text);
    auto color = text->Color();
    color.a = alpha;
    text->SetColor(color);
}

void TextFadeIn::Render(Vec2<Cart>) {}
