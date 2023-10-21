#include "InputManager.h"

#include "Game.h"
#include "util.h"

#define MODULE "InputManager"

inline static int mapkey(int key) {
  return key >= BLOCK1_START ? key - BLOCK1_START + BLOCK0_SIZE : key;
}

InputManager &InputManager::Instance() {
  static InputManager instance;
  return instance;
}

// static InputManager already zeros everything for us?
InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::Update() {
  SDL_GetMouseState(&mouseX, &mouseY);
  auto camera = Game::Instance().GetState().GetCamera().Pos();
  mouseX += camera.x;
  mouseY += camera.y;
  quitRequested = false;
  memset(keyUpdate, 0, sizeof(keyUpdate));
  memset(mouseUpdate, 0, sizeof(mouseUpdate));

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      quitRequested = true;
      break;
    case SDL_KEYDOWN:
      if (!event.key.repeat) {
        keyUpdate[mapkey(event.key.keysym.scancode)] = true;
        keyState[mapkey(event.key.keysym.scancode)] = InputState::Down;
      }
      break;
    case SDL_KEYUP:
      if (!event.key.repeat) {
        keyUpdate[mapkey(event.key.keysym.scancode)] = true;
        keyState[mapkey(event.key.keysym.scancode)] = InputState::Up;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouseUpdate[event.button.button] = true;
      mouseState[event.button.button] = InputState::Down;
      break;
    case SDL_MOUSEBUTTONUP:
      mouseUpdate[event.button.button] = true;
      mouseState[event.button.button] = InputState::Up;
      break;
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

int InputManager::MouseX() { return mouseX; }
int InputManager::MouseY() { return mouseY; }
Vec2<Cart> InputManager::Mouse() { return {(float)mouseX, (float)mouseY}; }

bool InputManager::QuitRequested() { return quitRequested; }
