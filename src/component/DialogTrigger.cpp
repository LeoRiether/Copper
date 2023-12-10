#include "component/DialogTrigger.h"

#include "Component.h"
#include "Game.h"
#include "Prefabs.h"
#include "component/Player.h"
#include "component/Text.h"
#include "physics/Tags.h"
#include "state/StageState.h"
#include "InputManager.h"

#define MODULE "DialogTrigger"

DialogTrigger::DialogTrigger(GameObject& go, std::string dialogFile) : Component(go)
	, dialogFile(dialogFile) {}

void DialogTrigger::NotifyCollision(GameObject& other) {
    auto& input = InputManager::Instance();
    auto& state = (StageState&)Game::Instance().GetState();

    if (other.tags.test(tag::Player)) {
		if (!(input.KeyPress(INTERACT_KEY) && state.playerMovement)) 
			return;
		Game::Instance().GetState().RequestAddObject
			(MakeDialog(dialogFile));
    }
}
