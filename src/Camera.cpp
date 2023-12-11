#include "Camera.h"

#include <utility>

#include "Game.h"
#include "InputManager.h"
#include "math/Interpolate.h"
#include "util.h"

#define MODULE "Camera"

void Camera::Follow(GameObject* newFocus) {
    focus = newFocus;
    focusChanged.Restart();
}

void Camera::Unfollow() {
    focus = nullptr;
    focusChanged.Restart();
}

void Camera::SecondaryFollow(std::weak_ptr<GameObject> go) {
    secondary.emplace_back(go);
    focusChanged.Restart();
}

void Camera::Update(float dt) {
    if (focus)
        updateFocus(dt);
    else
        updateFromInput(dt);

    updateShake();
}

void Camera::updateFocus(float dt) {
    const Vec2<Cart> screenCenter{(float)SCREEN_WIDTH / 2.0f,
                                  (float)SCREEN_HEIGHT / 2.0f};

    auto target = focus->box.Center();

    auto [sCoM, secN] = secondaryCenterOfMass();
    if (secN > 0) {
        target = target.lerp(sCoM, 0.3);
    }

    if (prevCoMN != secN) {
        prevCoMN = secN;
        focusChanged.Restart();
    }

    focusChanged.Update(dt);
    auto delta = target - screenCenter - pos;
    auto p = lerp(0.01, 0.1, std::min<float>(focusChanged.Get() * 2.0f, 1.0f));
    pos = pos + delta * p;
}

void Camera::updateFromInput(float dt) {
    auto& input = InputManager::Instance();
    if (input.IsKeyDown(RIGHT_ARROW_KEY)) pos.x += speed.x * dt;
    if (input.IsKeyDown(LEFT_ARROW_KEY)) pos.x -= speed.x * dt;
    if (input.IsKeyDown(DOWN_ARROW_KEY)) pos.y += speed.y * dt;
    if (input.IsKeyDown(UP_ARROW_KEY)) pos.y -= speed.y * dt;
}

void Camera::updateShake() {
    // Source: https://www.youtube.com/watch?v=tu-Qe66AvtY
    auto trauma = Game::Instance().Trauma();
    if (trauma > 0.01) {
        // static siv::BasicPerlinNoise<float> perlinX{rng()};
        // static siv::BasicPerlinNoise<float> perlinY{rng()};
        // static float t = 0;
        // t += dt;
        //
        // auto gen = [&](auto& perlin) {
        //     return 200.0f * trauma * trauma * perlin.octave1D(t, 8);
        // };
        // shakeDelta = {gen(perlinX), gen(perlinY)};

        auto gen = [&]() { return 100.0f * trauma * trauma * randf(-1, 1); };
        shakeDelta = {gen(), gen()};
    } else {
        shakeDelta = {0, 0};
    }
}

std::pair<Vec2<Cart>, int> Camera::secondaryCenterOfMass() {
    if (!focus) return {{0, 0}, 0};

    for (int i = 0; i < (int)secondary.size();) {
        if (secondary[i].expired()) {
            std::swap(secondary[i], secondary.back());
            secondary.pop_back();
        } else {
            i++;
        }
    }

    constexpr float radius = 612.0f;
    Vec2<Cart> sum{0, 0};
    int counted = 0;
    auto primaryCenter = focus->box.Center();
    for (auto& go : secondary) {
        auto center = go.lock()->box.Center();
        if ((center - primaryCenter).norm2() <= radius * radius) {
            sum = sum + center;
            counted++;
        }
    }

    if (counted == 0) return {{0, 0}, 0};
    return {sum * (1.0f / counted), counted};
}

Vec2<Cart> Camera::Pos() { return pos + shakeDelta; }

void Camera::SetPos(Vec2<Cart> p) {
    const Vec2<Cart> screenCenter{(float)SCREEN_WIDTH / 2.0f,
                                  (float)SCREEN_HEIGHT / 2.0f};
    pos = p - screenCenter;
}
