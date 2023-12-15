#include "state/EndState.h"

#include "Colors.h"
#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Resources.h"
#include "component/Sprite.h"
#include "component/Text.h"
#include "component/TextBlinker.h"
#include "component/TextFadeIn.h"
#include "state/StageState.h"
#include "util.h"

EndState::EndState() {}

EndState::~EndState() {}

void EndState::LoadAssets() {
  // uhm, I'm doing everything in EndState::Start()...
}

void EndState::Update(float dt) {
  ProcessAddRequests();

  auto &input = InputManager::Instance();
  input.Update();

  if (input.QuitRequested() || input.KeyPress(SDL_SCANCODE_ESCAPE)) {
    quitRequested = true;
    return;
  }

  if (input.KeyPress(SDL_SCANCODE_SPACE)) {
    GameData::playerHp = 100;
    Game::Instance().RequestPop();
    Game::Instance().RequestPush(new StageState{});
  }

  UpdateArray(dt);
}

void EndState::Render() { RenderArray(); }

void EndState::Start() {

  {
    auto text = GameData::playerVictory?"VICTORY":"DEFEAT";
    auto go = new GameObject{};
    go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 120,
                              Text::Blended, text,
                              colorFromHex("CB6D51")});
    go->AddComponent(new TextFadeIn{*go, 1.0f});
    go->box.SetCenter({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 50});
    RequestAddObject(go);
  }

  if (GameData::playerVictory) {
    // (y)
  } else {
    {
      auto go = new GameObject{};
      auto& input = InputManager::Instance();
      auto text = "try again press SPACE";
      go->AddComponent(new Text{*go, ASSETS "/font/THEROOTS.TTF", 30,
                                Text::Blended, text, colorFromHex("dda08d")});
      go->AddComponent(new TextBlinker{*go, 4.0f});
      go->box.SetCenter({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 140});
      RequestAddObject(go);
    }
  }

  // bgMusic.Play();

  // bgGO->box.x = 0;
  // bgGO->box.y = 0;
  // RequestAddObject(bgGO);

}

void EndState::Pause() { bgMusic.Stop(); }

void EndState::Resume() { bgMusic.Play(); }
