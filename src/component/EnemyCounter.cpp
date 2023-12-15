#include "component/EnemyCounter.h"

#include "Colors.h"
#include "component/Animation.h"
#include "component/Text.h"
#include "Game.h"
#include "GameData.h"
#include "state/StageState.h"
#include "util.h"

#define MODULE "EnemyCounter"

EnemyCounter::EnemyCounter(GameObject& go, bool gameData): Component(go), gameData(gameData){
    auto& state = (StageState&)Game::Instance().GetState();
	auto pos = Vec2<Cart>{SCREEN_WIDTH-127, 80};
	enemyCount = gameData? GameData::enemiesKilled : state.EnemyCount;
	log2("%d", enemyCount);
    auto text = new Text{go,
                         ASSETS "/font/AldotheApache.ttf",
                         30,
                         Text::Blended,
                         std::to_string(enemyCount),
                         colorFromHex("#FF0000"),
						 state.EnemyCount,
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
	enemyCount = gameData ? GameData::enemiesKilled : state.EnemyCount;
	auto text = (Text*)associated.GetComponent(CType::Text);
	text->SetText(std::to_string(enemyCount));
}
