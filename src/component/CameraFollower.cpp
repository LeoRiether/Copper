#include "component/CameraFollower.h"

#include "Game.h"

CameraFollower::CameraFollower(GameObject& go) : Component(go), go(go) {}

void CameraFollower::Update(float) {
    Vec2<Cart> camera =
        Game::Instance().GetState().GetCamera().Pos();  // indirection!
    go.box.x = camera.x;
    go.box.y = camera.y;
}

void CameraFollower::Render(Vec2<Cart>) {}

bool CameraFollower::Is(CType type) { return type == CType::CameraFollower; }
