#pragma once
#include "SDL_include.h"
#include "Vec2.h"

constexpr int KEYS = 0x7F + 0x11A;  // A soma dos tamanhos dos blocos de teclas
constexpr int BLOCK0_SIZE = 0x7F;
constexpr int BLOCK1_START = 0x3FFFFF81;

constexpr int LEFT_ARROW_KEY = SDL_SCANCODE_LEFT;
constexpr int RIGHT_ARROW_KEY = SDL_SCANCODE_RIGHT;
constexpr int UP_ARROW_KEY = SDL_SCANCODE_UP;
constexpr int DOWN_ARROW_KEY = SDL_SCANCODE_DOWN;
constexpr int ESCAPE_KEY = SDL_SCANCODE_ESCAPE;
constexpr int LEFT_MOUSE_BUTTON = SDL_BUTTON_LEFT;
constexpr int RIGHT_MOUSE_BUTTON = SDL_BUTTON_RIGHT;

constexpr int MOVE_FORWARD_KEY = SDL_SCANCODE_W;
constexpr int MOVE_BACKWARDS_KEY = SDL_SCANCODE_S;
constexpr int ROTATE_CLOCKWISE_KEY = SDL_SCANCODE_D;
constexpr int ROTATE_COUNTERCLOCKWISE_KEY = SDL_SCANCODE_A;

enum InputState { Up = false, Down = true };

class InputManager {
   private:
    InputState mouseState[6];
    int mouseUpdate[6];
    InputState keyState[KEYS];
    int keyUpdate[KEYS];
    bool quitRequested;
    int updateCounter;
    int mouseX, mouseY;

    InputManager();
    ~InputManager();

   public:
    static InputManager& Instance();

    void Update();
    bool KeyPress(int key);
    bool KeyRelease(int key);
    bool IsKeyDown(int key);

    bool MousePress(int button);
    bool MouseRelease(int button);
    bool IsMouseDown(int button);

    int MouseX();
    int MouseY();

    bool QuitRequested();
};
