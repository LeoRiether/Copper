#include "state/EndState.h"

#include "Colors.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Prefabs.h"
#include "Resources.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "component/TextBlinker.h"
#include "component/TextFadeIn.h"
#include "component/CoinCounter.h"
#include "component/EnemyCounter.h"
#include "state/StageState.h"
#include "util.h"

EndState::EndState() {}

EndState::~EndState() {}

void EndState::LoadAssets() {
  // uhm, I'm doing everything in EndState::Start()...
}

void EndState::Update(float dt) {
  if(leave) {
    timer += dt;
    if(timer>0.2) {
      GameData::playerHp = 100;
      Game::Instance().RequestPop();
      Game::Instance().RequestPush(new StageState{});
    }
    return;
  }

  ProcessAddRequests();

  auto &input = InputManager::Instance();
  input.Update();

  if (input.QuitRequested() || input.KeyPress(SDL_SCANCODE_ESCAPE)) {
    quitRequested = true;
    return;
  }

  if (input.KeyPress(SDL_SCANCODE_SPACE)) {
    leave = true;
    RequestAddObject(MakeStageTransitionDimmer_FadeOut());
  }

  UpdateArray(dt);
}

void EndState::Render() { RenderArray(); }

void EndState::Start() {



  if (GameData::playerVictory) {
    {
      auto bgGO = new GameObject{};
      auto sprite = new Sprite{*bgGO, ASSETS "/img/Tela_vitoria.png"};
      sprite->SetScale((float)SCREEN_WIDTH / sprite->SheetWidth());
      bgGO->AddComponent(sprite);
      RequestAddObject(bgGO);
    }
  } else {
    {
      auto bgGO = new GameObject{};
      auto sprite = new Sprite{*bgGO, ASSETS "/img/Tela derrota.png"};
      sprite->SetScale((float)SCREEN_WIDTH / sprite->SheetWidth());
      bgGO->AddComponent(sprite);
      RequestAddObject(bgGO);
    }
    {
      auto go = new GameObject{};
      auto& input = InputManager::Instance();
      auto text = "try again press SPACE";
      go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 20,
                                Text::Blended, text, colorFromHex("dda08d")});
      go->AddComponent(new TextBlinker{*go, 4.0f});
      go->box.SetCenter({SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f - 110});
      RequestAddObject(go);
    }
  }

  {
    auto text = GameData::playerVictory?"VICTORY":"DEFEAT";
    auto go = new GameObject{};
    go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 100,
                              Text::Blended, text,
                              colorFromHex("CB6D51")});
    go->AddComponent(new TextFadeIn{*go, 1.0f});
    go->box.SetCenter({SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f - 200});
    RequestAddObject(go);
  }

  // {
  //   auto coinCounterGo = new GameObject{};
  //   coinCounterGo->AddComponent(new CoinCounter{*coinCounterGo, {SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f + 100}});
  //   coinCounterGo->box.SetCenter({SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f + 100});
  //   RequestAddObject(coinCounterGo);

  //   EnemyCount = GameData::enemiesKilled;
  //   auto enemyCountergo = new GameObject{};
  //   enemyCountergo->AddComponent(new EnemyCounter{*enemyCountergo, {SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f + 100}});
  //   coinCounterGo->box.SetCenter({SCREEN_WIDTH / 2.0f + 250, SCREEN_HEIGHT / 2.0f + 100});
  //   RequestAddObject(enemyCountergo);
  // }

  // bgMusic.Play();

  // bgGO->box.x = 0;
  // bgGO->box.y = 0;
  // RequestAddObject(bgGO);

}

void EndState::Pause() { bgMusic.Stop(); }

void EndState::Resume() { bgMusic.Play(); }
