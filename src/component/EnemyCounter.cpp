#include "component/EnemyCounter.h"

#include "Colors.h"
#include "component/Animation.h"
#include "component/Text.h"
#include "Game.h"
#include "state/StageState.h"
#include "util.h"

#define MODULE "EnemyCounter"

EnemyCounter::EnemyCounter(GameObject& go, Vec2<Cart> pos): Component(go){
    auto& state = (StageState&)Game::Instance().GetState();
    auto text = new Text{go,
                         ASSETS "/font/AldotheApache.ttf",
                         30,
                         Text::Blended,
                         std::to_string(state.EnemyCount),
                         colorFromHex("#FF0000"),
						 0,
						 true};
    go.AddComponent(text);
	go.box.SetCenter({SCREEN_WIDTH-50, 107});
	go.renderLayer = 200;
	auto skull = (new GameObject{})->WithCenterAt(pos);
	auto skullSprite = new Sprite{*skull,
		ASSETS "/img/skull.png", true};
	skullSprite->SetScale(1.2);
	auto animation = Animation::horizontal(*skull,
			*skullSprite,
			24, 0.05);
	animation->Play("default", true);
	skull->AddComponent(skullSprite);
	skull->AddComponent(animation);
	skull->renderLayer = 200;
	go.RequestAdd(skull);
}

void EnemyCounter::Update(float) {
    auto& state = (StageState&)Game::Instance().GetState();
	auto text = (Text*)associated.GetComponent(CType::Text);
	text->SetText(std::to_string(state.EnemyCount));
}
