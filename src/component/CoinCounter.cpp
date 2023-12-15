#include "component/CoinCounter.h"

#include "Colors.h"
#include "component/Animation.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "Game.h"
#include "util.h"

#define MODULE "CoinCounter"

int CoinCounter::coins = 0;
int CoinCounter::target = 0;
CoinCounter* CoinCounter::coinCounter = nullptr;
CoinCounter::CoinCounter(GameObject& go): Component(go){
	coins = 0;
	target = 0;
    auto text = new Text{go,
                         ASSETS "/font/AldotheApache.ttf",
                         30,
                         Text::Blended,
                         "0",
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
	if (coins < target){
		auto text = (Text*)associated.GetComponent(CType::Text);
		text->SetText(std::to_string(++coins));
	}
}

void CoinCounter::AddCoin() {
	target = target + 5;
}

