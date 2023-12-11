#include "component/Tooltip.h"

#include "Colors.h"
#include "component/Player.h"
#include "component/Text.h"
#include "util.h"

#define MODULE "Tooltip"

Tooltip* Tooltip::tooltip;

Tooltip::Tooltip(GameObject& go): Component(go) {
	Tooltip::tooltip = this;
    auto text = new Text{go, ASSETS "/font/THEROOTS.TTF", 25,
                              Text::Wrapped, "?", colorFromHex("4D2011")};
	text->SetAlpha(0);
	go.AddComponent(text);
	auto player = Player::player;
	if (player)
		go.box.SetFoot(player->associated.box.Head());
}


void Tooltip::Update(float dt){
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
        text->SetAlpha(lerp((fadingTime - fadingOut) / fadingTime, 255, 0));
        fadingOut -= dt;
        if (fadingOut <= 0){
			text->SetAlpha(0);
			invis = true;
		}
    } else if (fadingIn > 0) {
        text->SetAlpha(lerp((fadingTime - fadingIn) / fadingTime, 0, 255));
        fadingIn -= dt;
        if (fadingIn <= 0){ 
            text->SetAlpha(255);
			invis = false;
		}
    }
}

void Tooltip::SetText(std::string str){
    auto text = (Text*)associated.GetComponent(CType::Text);
    if (!text) fail("no associated Text");
	text->SetText(str);
}

void Tooltip::FadeIn(){
	if (fadingIn > 0 || !invis)
		return;
	if (fadingOut > 0){
		fadingIn = fadingOut;
		fadingOut = 0;
		return;
	}
	fadingIn = fadingTime;
	fadingOut = 0;
}

void Tooltip::FadeOut(){
	if (fadingOut > 0 || invis)
		return;
	if (fadingIn > 0){
		fadingOut = fadingIn;
		fadingIn = 0;
		return;
	}
	fadingOut = fadingTime;
	fadingIn = 0;
}
