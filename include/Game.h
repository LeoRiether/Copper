#pragma once
#include <memory>
#include <string>
#include <vector>

#include "SDL_include.h"
#include "State.h"
#include "util.h"
using std::string;
using std::unique_ptr;
using std::vector;

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 600;

class Game {
   private:
    // Singletons are just global variables with extra steps
    static Game* instance;
    SDL_Window* window;
    SDL_Renderer* renderer;
    vector<unique_ptr<State>> stateStack;

    uint32_t frameStart;
    float dt;

    Game(const char* title, int width, int height);

    void CalculateDeltaTime();

   public:
    ~Game();

    void Run();
    void Push(State* state);

    float DeltaTime();
    SDL_Renderer* Renderer();
    State& GetState();
    static Game& Instance();
};
