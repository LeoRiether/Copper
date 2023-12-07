#include "InputManager.h"

#include "Game.h"
#include "util.h"

#define MODULE "InputManager"

inline static int mapkey(int key) {
    return key >= BLOCK1_START ? key - BLOCK1_START + BLOCK0_SIZE : key;
}

InputManager& InputManager::Instance() {
    static InputManager instance;
    return instance;
}

// static InputManager already zeros everything for us?
InputManager::InputManager() {
    // Find GameController
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            special("GameController detected");
            controller = SDL_GameControllerOpen(i);
            break;
        }
    }
}

InputManager::~InputManager() {
    if (controller) {
        SDL_GameControllerClose(controller);
    }
}

void InputManager::Update() {
    auto camera = Game::Instance().GetState().GetCamera().Pos();
    memset(mouseUpdate, 0, sizeof(mouseUpdate));
    mouseWheel = MouseWheelState::None;
    memset(keyUpdate, 0, sizeof(keyUpdate));
    quitRequested = false;
    SDL_GetMouseState(&rawMouseX, &rawMouseY);
    mouseX = rawMouseX + camera.x;
    mouseY = rawMouseY + camera.y;
    memset(controllerUpdate, 0, sizeof(controllerUpdate));

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                quitRequested = true;
                break;
            }
            case SDL_KEYDOWN: {
                if (!event.key.repeat) {
                    keyUpdate[mapkey(event.key.keysym.scancode)] = true;
                    keyState[mapkey(event.key.keysym.scancode)] =
                        InputState::Down;
                }
                break;
            }
            case SDL_KEYUP: {
                if (!event.key.repeat) {
                    keyUpdate[mapkey(event.key.keysym.scancode)] = true;
                    keyState[mapkey(event.key.keysym.scancode)] =
                        InputState::Up;
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                mouseUpdate[event.button.button] = true;
                mouseState[event.button.button] = InputState::Down;
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                mouseUpdate[event.button.button] = true;
                mouseState[event.button.button] = InputState::Up;
                break;
            }
            case SDL_MOUSEWHEEL: {
                mouseWheel = (MouseWheelState)event.wheel.y;
                break;
            }
            case SDL_CONTROLLERBUTTONDOWN: {
                controllerUpdate[event.cbutton.button] = true;
                controllerState[event.cbutton.button] = InputState::Down;
                break;
            }
            case SDL_CONTROLLERBUTTONUP: {
                controllerUpdate[event.cbutton.button] = true;
                controllerState[event.cbutton.button] = InputState::Up;
                break;
            }
            // case SDL_CONTROLLERAXISMOTION: {
            //     constexpr int mx = 32768;
            //     constexpr int dead = 20'000;
            //     axis[event.caxis.axis] =
            //         float(event.caxis.value - dead) / float(mx - dead);
            //     break;
            // }
            default:
                break;
        }
    }
}

bool InputManager::KeyPress(int key) {
    return keyUpdate[mapkey(key)] && keyState[mapkey(key)] == InputState::Down;
}

bool InputManager::KeyRelease(int key) {
    return keyUpdate[mapkey(key)] && keyState[mapkey(key)] == InputState::Up;
}

bool InputManager::IsKeyDown(int key) {
    return keyState[mapkey(key)] == InputState::Down;
}

bool InputManager::MousePress(int button) {
    return mouseUpdate[button] && mouseState[button] == InputState::Down;
}

bool InputManager::MouseRelease(int button) {
    return mouseUpdate[button] && mouseState[button] == InputState::Up;
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button] == InputState::Down;
}

bool InputManager::ControllerPress(int key) {
    return controllerUpdate[key] && controllerState[key] == InputState::Down;
}

bool InputManager::ControllerRelease(int key) {
    return controllerUpdate[key] && controllerState[key] == InputState::Up;
}

bool InputManager::IsControllerDown(int key) {
    return controllerState[key] == InputState::Down;
}

float InputManager::Axis(SDL_GameControllerAxis ax) {
    return mapAxis(SDL_GameControllerGetAxis(controller, ax));
}

Vec2<Cart> InputManager::AxisVec(int ax) {
    if (ax == -1) {
        return Vec2<Cart>{Axis(SDL_CONTROLLER_AXIS_LEFTX),
                          Axis(SDL_CONTROLLER_AXIS_LEFTY)}
            .normalize();
    }
    return Vec2<Cart>{Axis(SDL_CONTROLLER_AXIS_RIGHTX),
                      Axis(SDL_CONTROLLER_AXIS_RIGHTY)}
        .normalize();
}

float InputManager::mapAxis(int x) {
    constexpr int mx = 32'768;
    constexpr int dead = 5'000;

    if (abs(x) <= dead) return 0;

    if (x > 0) return float(x - dead) / (mx - dead);
    return float(x + dead) / (mx - dead);
}

MouseWheelState InputManager::MouseWheel() { return mouseWheel; }

int InputManager::MouseX() { return mouseX; }
int InputManager::MouseY() { return mouseY; }
Vec2<Cart> InputManager::Mouse() { return {(float)mouseX, (float)mouseY}; }
Vec2<Cart> InputManager::RawMouse() {
    return {(float)rawMouseX, (float)rawMouseY};
}

bool InputManager::QuitRequested() { return quitRequested; }
