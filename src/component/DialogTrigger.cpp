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

void DialogTrigger::NotifyCollision(GameObject& other) {
    auto& input = InputManager::Instance();
    auto& state = (StageState&)Game::Instance().GetState();
    auto tooltip = Tooltip::tooltip;

	if (!((input.KeyPress(INTERACT_KEY) || input.ControllerPress(SDL_CONTROLLER_BUTTON_Y)) && state.playerMovement)) 
		return;
    if (other.tags.test(tag::Player)) {
		Game::Instance().GetState().RequestAddObject
			(MakeDialog(dialogFile));
    }
}

void DialogTrigger::NotifyCollisionEnter(GameObject& other) {
	if (other.tags.test(tag::Player)) {
    auto tooltip = Tooltip::tooltip;
	auto& input = InputManager::Instance();
	tooltip->SetText(input.HasController() ? "TRIANGLE" : "E");
	tooltip->FadeIn();
	}
}

void DialogTrigger::NotifyCollisionLeave(GameObject& other) {
	if (other.tags.test(tag::Player)) {
    auto tooltip = Tooltip::tooltip;
	tooltip->FadeOut();
	}
}
