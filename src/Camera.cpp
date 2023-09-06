#include "Camera.h"

#include "Game.h"
#include "InputManager.h"

void Camera::Follow(GameObject* newFocus) { focus = newFocus; }

void Camera::Unfollow() { focus = nullptr; }

void Camera::Update(float dt) {
    if (focus) {
        const Vec2 screen{(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT};
        const Vec2 boxsize{focus->box.w, focus->box.h};
        pos = (screen - boxsize) / 2.0f;
    } else {
        auto& input = InputManager::Instance();
        if (input.IsKeyDown(RIGHT_ARROW_KEY)) pos.x += speed.x * dt;
        if (input.IsKeyDown(LEFT_ARROW_KEY)) pos.x -= speed.x * dt;
        if (input.IsKeyDown(DOWN_ARROW_KEY)) pos.y += speed.y * dt;
        if (input.IsKeyDown(UP_ARROW_KEY)) pos.y -= speed.y * dt;
    }
}