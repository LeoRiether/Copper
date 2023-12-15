#include "component/HitMarker.h"

#include "Colors.h"
#include "component/Text.h"
#include "util.h"

#define MODULE "HitMarker"

HitMarker::HitMarker(GameObject& go, int dmg) : Component(go) {
    go.renderLayer = 3;
    auto text = new Text{go,
                         ASSETS "/font/AldotheApache.ttf",
                         30,
                         Text::Blended,
                         std::to_string(dmg),
                         colorFromHex("#F9E0D9")};
    go.AddComponent(text);
}

void HitMarker::Update(float dt) {
    // I'm sure this integration method has a name
    // I just don't know which one
    speed.y += g * dt;
    associated.box.OffsetBy(speed * dt);

    alpha -= 3;
    const auto& texts = associated.GetAllComponents(CType::Text);
    for (auto& text : texts) {
        ((Text*)text.get())->SetAlpha(alpha);
    }
    if (alpha <= 50) associated.RequestDelete();
}
