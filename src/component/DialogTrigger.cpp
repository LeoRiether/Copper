#include "component/DialogTrigger.h"

#include "Component.h"
#include "Game.h"
#include "Prefabs.h"
#include "component/Player.h"
#include "component/Text.h"
#include "component/Tooltip.h"
#include "physics/Tags.h"
#include "state/StageState.h"
#include "InputManager.h"
#include "util.h"

#define MODULE "DialogTrigger"

DialogTrigger::DialogTrigger(GameObject& go, std::string dialogFile) : Component(go)
	, dialogFile(dialogFile) {}

void DialogTrigger::Update(float){
    if (!collisionLastFrame){
       auto tooltip = Tooltip::tooltip;
       tooltip->FadeOut();
    }
	collisionLastFrame = false;
}

void DialogTrigger::NotifyCollision(GameObject& other) {
    auto& input = InputManager::Instance();
    auto& state = (StageState&)Game::Instance().GetState();
    auto tooltip = Tooltip::tooltip;

    if (other.tags.test(tag::Player)) {
      collisionLastFrame = true;
      tooltip->SetText("E");
      tooltip->FadeIn();
      if (!(input.KeyPress(INTERACT_KEY) && state.playerMovement)) 
          return;
		Game::Instance().GetState().RequestAddObject
			(MakeDialog(dialogFile));

    }
}

