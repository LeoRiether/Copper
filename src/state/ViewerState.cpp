#include "state/ViewerState.h"

#include "Consts.h"
#include "Game.h"
#include "InputManager.h"
#include "SDL_render.h"
#include "SDL_scancode.h"
#include "component/Collider.h"
#include "component/Sprite.h"
#include "util.h"

#define MODULE "ViewerState"

/*
 * This is pretty bad code I think?
 * Sorry
 * It's not too important anyway, I think
 */

void ViewerState::LoadAssets() {}

void ViewerState::Start() {
    {
        GameObject* go = new GameObject{};
        auto sprite = new Sprite{*go, Consts::GetString("viewer.path")};
        if (sprite->SheetWidth() + 2 >= SCREEN_WIDTH)
            zoom = std::min(
                zoom, (float)SCREEN_WIDTH / ((float)sprite->SheetWidth() + 2));
        if (sprite->SheetHeight() + 2 >= SCREEN_HEIGHT)
            zoom = std::min(zoom, (float)SCREEN_HEIGHT /
                                      ((float)sprite->SheetHeight() + 2));
        go->AddComponent(sprite);
        go->AddComponent(new Collider{*go});
        image = RequestAddObject(go);
    }
}

void ViewerState::Pause() {}
void ViewerState::Resume() {}

void ViewerState::Update(float dt) {
    ProcessAddRequests();

    auto& input = InputManager::Instance();
    input.Update();

    if (input.QuitRequested()) {
        quitRequested = true;
        return;
    }

    if (input.KeyPress(SDL_SCANCODE_ESCAPE)) {
        Game::Instance().RequestPop();
        return;
    }

    if (input.KeyPress(SDL_SCANCODE_M)) {
        mode = mode == "iso" ? "cart" : "iso";
    }

    if (input.KeyPress(SDL_SCANCODE_MINUS)) {
        zoom *= 0.8;
    } else if (input.KeyPress(SDL_SCANCODE_EQUALS)) {
        zoom /= 0.8;
    }

    if (input.KeyPress(SDL_SCANCODE_RETURN)) {
        warn2("rect = { %g, %g, %g, %g }", dragRect.x, dragRect.y, dragRect.w,
              dragRect.h);
    }

    UpdateArray(dt);

    auto imageGO = image.lock();
    auto sprite = (Sprite*)imageGO->GetComponent(CType::Sprite);
    sprite->SetScale(zoom);
    imageGO->box.w = sprite->SheetWidth() * zoom;
    imageGO->box.h = sprite->SheetHeight() * zoom;
    imageGO->box.SetCenter({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f});

    if (input.MouseRelease(1)) {
        drag = {};
    } else if (input.IsMouseDown(1)) {
        if (drag.has_value()) {
            drag->to = input.RawMouse();
        } else {
            drag = {input.RawMouse(), input.RawMouse()};
        }
    }

    if (input.IsKeyDown(SDL_SCANCODE_SPACE)) {
        camera->SetPos(camera->Pos() + lastMousePos - input.RawMouse());
    }

    lastMousePos = input.RawMouse();
}

void ViewerState::Render() {
    SDL_Renderer* renderer = Game::Instance().Renderer();

    auto rect = [&](SDL_Rect r) { SDL_RenderFillRect(renderer, &r); };
    auto around = [&](Vec2<Cart> point) {
        rect({(int)point.x - 5, (int)point.y - 5, 10, 10});
    };
    auto line = [&](Vec2<Cart> from, Vec2<Cart> to) {
        SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y);
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    rect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

    RenderArray();

    if (drag) {
        auto& [from, to] = *drag;

        Vec2<Cart> left, right;
        if (mode == "iso") {
            auto cam = camera->Pos().toIso();
            auto pivot = image.lock()->box.TopLeft().toIso();
            // to.x = from.x;
            auto fromIso = from.toIso();
            auto toIso = to.toIso();
            left = Vec2<Iso>{fromIso.x, std::min(fromIso.y, toIso.y)}.toCart();
            right = Vec2<Iso>{toIso.x, std::max(fromIso.y, toIso.y)}.toCart();
            dragRect = {(toIso.x - pivot.x + cam.x) / zoom,
                        (toIso.y - pivot.y + cam.y) / zoom,
                        (fromIso.x - toIso.x) / zoom,
                        (fromIso.y - toIso.y) / zoom};
        } else {
            auto cam = camera->Pos();
            auto pivot = image.lock()->box.TopLeft();
            left = {from.x, std::max(from.y, to.y)};
            right = {to.x, std::min(from.y, to.y)};
            dragRect = {(from.x - pivot.x + cam.x) / zoom,
                        (from.y - pivot.y + cam.y) / zoom,
                        (to.x - from.x) / zoom, (to.y - from.y) / zoom};
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        line(from, to);
        line(left, right);
        line(from, left);
        line(left, to);
        line(to, right);
        line(right, from);
        around(from);
        around(to);
        around(left);
        around(right);
    }
}
