#pragma once
#include "SDL_include.h"
#include "Vec2.h"

constexpr int KEYS = 0x7F + 0x11A;  // A soma dos tamanhos dos blocos de teclas
constexpr int BLOCK0_SIZE = 0x7F;
constexpr int BLOCK1_START = 0x3FFFFF81;

#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define ESCAPE_KEY SDLK_ESCAPE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT

enum InputState {
    Up = false, Down = true
};

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

    void Update(Vec2 camera);
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
