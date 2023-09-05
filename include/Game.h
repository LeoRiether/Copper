#pragma once
#include <string>

#include "SDL_include.h"
#include "State.h"
#include "util.h"
using std::string;

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 600;

class Game {
   private:
    // Singletons are just global variables with extra steps
    static Game* instance;
    SDL_Window* window;
    SDL_Renderer* renderer;
    State* state;

    uint32_t frameStart;
    float dt;

    Game(const char* title, int width, int height);

    void CalculateDeltaTime();

   public:
    ~Game();

    void Run();

    float DeltaTime();
    SDL_Renderer* Renderer();
    State& GetState();
    static Game& Instance();
};
