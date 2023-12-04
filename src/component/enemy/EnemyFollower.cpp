#include "component/enemy/EnemyFollower.h"

#include "Game.h"
#include "component/Animation.h"
#include "component/IsoCollider.h"
#include "component/Player.h"
#include "physics/Steering.h"
#include "util.h"

#define MODULE "EnemyFollower"

EnemyFollower::EnemyFollower(GameObject& go) : Component(go) {}

EnemyFollower* EnemyFollower::WithRobotCan(RobotCan* rc) {
    self = rc;
    return this;
}

void EnemyFollower::Update(float dt) {
    if (!Player::player) return;

    auto allAnimsPlay = [&](const string& id, bool loops = true) {
        auto& anims = associated.GetAllComponents(CType::Animation);
        for (auto& anim : anims) {
            ((Animation*)anim.get())->SoftPlay(id, loops);
        }
    };

    auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
    if (!iso) fail("companion without IsoCollider...");
    auto selfPos = iso->box.Center().transmute<Iso>();
    auto maybePlayerPos = Player::player->LookForMe(iso->box);

    if (!maybePlayerPos) {
        allAnimsPlay("hide_" + self->direction.toString(), false);
        return;
    }

    moveDelta = (*maybePlayerPos - selfPos).toCart().normalize() +
                Steering{}.AddTerrain(selfPos)->Result().toCart();
    moveDelta = moveDelta.normalize();

    self->direction = Direction::approxFromVec(moveDelta);
    allAnimsPlay("walk_" + self->direction.toString());

    auto realDistVec =
        Player::player->associated.box.Center() - selfPos.toCart();
    if (realDistVec.norm2() >= self->stopDistance * self->stopDistance) {
        self->associated.box.OffsetBy(moveDelta * speed * dt);
    }
}

//////////////////////////
//        Render        //
//////////////////////////
void EnemyFollower::Render(Vec2<Cart> camera) {
    static int& showDirections = Consts::GetInt("debug.show_directions");
    if (showDirections) {
        auto iso = (IsoCollider*)associated.GetComponent(CType::IsoCollider);
        if (!iso) fail("no IsoCollider");
        auto from = iso->box.Center().transmute<Iso>().toCart() - camera;
        auto to = from + moveDelta * 80.0f;

        auto renderer = Game::Instance().Renderer();
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawLineF(renderer, from.x, from.y, to.x, to.y);
    }
}
