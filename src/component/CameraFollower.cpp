#include "component/CameraFollower.h"

#include "Game.h"

CameraFollower::CameraFollower(GameObject &go, Vec2<Cart> position)
    : Component(go), go(go) {
  this->position = position;
}

void CameraFollower::Update(float) {
  Vec2<Cart> camera =
      Game::Instance().GetState().GetCamera().Pos(); // indirection!
  go.box.x = camera.x + position.x;
  go.box.y = camera.y + position.y;
}

void CameraFollower::Render(Vec2<Cart>) {}
