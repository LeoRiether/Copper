#include "component/EndOfStageTrigger.h"

#include "Component.h"
#include "Game.h"
#include "Prefabs.h"
#include "component/Player.h"
#include "physics/Tags.h"
#include "state/StageState.h"

#define MODULE "EndOfStageTrigger"

EndOfStageTrigger::EndOfStageTrigger(GameObject& go) : Component(go) {}

void EndOfStageTrigger::Update(float dt) {
    if (!active) return;

    timer += dt;
    if (timer >= 0.7) {
        Game::Instance().RequestPop();
        Game::Instance().RequestPush(new StageState{});
    }
}

void EndOfStageTrigger::NotifyCollision(GameObject& other) {
    if (other.tags.test(tag::Player) && !active) {
        // Start countdown!
        active = true;
        Player::player->ChangeState(Player::StageTransition);
        associated.RequestAdd(MakeStageTransitionDimmer_FadeOut());
    }
}
