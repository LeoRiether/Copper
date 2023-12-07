#pragma once
#include "SDL_include.h"
#include "math/Vec2.h"

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

constexpr int MOVE_UP_KEY = SDL_SCANCODE_W;
constexpr int MOVE_LEFT_KEY = SDL_SCANCODE_A;
constexpr int MOVE_DOWN_KEY = SDL_SCANCODE_S;
constexpr int MOVE_RIGHT_KEY = SDL_SCANCODE_D;

constexpr int DASH_KEY = SDL_SCANCODE_SPACE;

constexpr int NEXT_KEY = SDL_SCANCODE_N;
enum class InputState { Up = false, Down = true };

enum class MouseWheelState { Down = -1, None = 0, Up = 1 };

class InputManager {
   private:
    InputState mouseState[6];
    int mouseUpdate[6];
    MouseWheelState mouseWheel;
    InputState keyState[KEYS];
    int keyUpdate[KEYS];
    bool quitRequested;
    int rawMouseX, rawMouseY;
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

    MouseWheelState MouseWheel();

    int MouseX();
    int MouseY();
    Vec2<Cart> Mouse();
    Vec2<Cart> RawMouse();

    bool QuitRequested();
};
