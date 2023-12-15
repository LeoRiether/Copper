#include "component/CoinCounter.h"

#include "Colors.h"
#include "component/Animation.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "Game.h"
#include "GameData.h"
#include "util.h"

#define MODULE "CoinCounter"

CoinCounter* CoinCounter::coinCounter = nullptr;
CoinCounter::CoinCounter(GameObject& go): Component(go){
	target = GameData::coins;
    auto text = new Text{go,
                         ASSETS "/font/AldotheApache.ttf",
                         30,
                         Text::Blended,
                         std::to_string(GameData::coins),
                         colorFromHex("#FF0000"),
						 0,
						 true};
    go.AddComponent(text);
	go.box.SetCenter({SCREEN_WIDTH-50, 67});
	go.renderLayer = 200;
	auto coin = (new GameObject{})->WithCenterAt(Vec2<Cart>{SCREEN_WIDTH-120, 50});
	auto coinSprite = new Sprite{*coin,
		ASSETS "/img/lego_studs.png", true};
	coinSprite->SetScale(0.5);
	auto animation = Animation::horizontal(*coin,
			*coinSprite,
			32, 0.05);
	animation->Play("default", true);
	coin->AddComponent(coinSprite);
	coin->AddComponent(animation);
	coin->renderLayer = 200;
	go.RequestAdd(coin);
	coinCounter = this;
}

void CoinCounter::Update(float) {
	if (GameData::coins < target){
		auto text = (Text*)associated.GetComponent(CType::Text);
		text->SetText(std::to_string(++GameData::coins));
	}
}

void CoinCounter::AddCoin() {
	target = target + 5;
}

