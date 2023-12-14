#include "component/CoinCounter.h"

#include "Colors.h"
#include "component/Animation.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "Game.h"
#include "util.h"

#define MODULE "CoinCounter"

int CoinCounter::coins = 0;
CoinCounter* CoinCounter::coinCounter = nullptr;
CoinCounter::CoinCounter(GameObject& go): Component(go){
    auto text = new Text{go,
                         ASSETS "/font/AldotheApache.ttf",
                         30,
                         Text::Blended,
                         "1000",
                         colorFromHex("#FF0000"),
						 true};
    go.AddComponent(text);
//	go.box.SetCenter({SCREEN_WIDTH-150, 200});
	go.box.SetCenter({ 479, 10156 });
	go.renderLayer = 200;
	auto coin = (new GameObject{})->WithCenterAt(Vec2<Cart>{638, 10169});//SCREEN_WIDTH-200, 200});
	auto coinSprite = new Sprite{*coin,
		ASSETS "/img/lego_studs.png", false};
	coinSprite->SetScale(0.1);
	auto animation = Animation::horizontal(*coin,
			*coinSprite,
			32, 0.2);
	animation->Play("default", true);
	go.RequestAdd(coin);
	coin->renderLayer = 200;
	coinCounter = this;
}

void CoinCounter::AddCoins(int c) {
	
}

void CoinCounter::Render(){}
