#include "Camera.h"

#include "Game.h"
#include "InputManager.h"
#include "util.h"

#define MODULE "Camera"

void Camera::Follow(GameObject* newFocus) { focus = newFocus; }

void Camera::Unfollow() { focus = nullptr; }

void Camera::Update(float dt) {
    if (focus) {
        const Vec2<Cart> screenCenter{(float)SCREEN_WIDTH / 2.0f,
                                      (float)SCREEN_HEIGHT / 2.0f};
        pos = focus->box.Center() - screenCenter;
    } else {
        auto& input = InputManager::Instance();
        if (input.IsKeyDown(RIGHT_ARROW_KEY)) pos.x += speed.x * dt;
        if (input.IsKeyDown(LEFT_ARROW_KEY)) pos.x -= speed.x * dt;
        if (input.IsKeyDown(DOWN_ARROW_KEY)) pos.y += speed.y * dt;
        if (input.IsKeyDown(UP_ARROW_KEY)) pos.y -= speed.y * dt;
    }
}
