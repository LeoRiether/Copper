#pragma once
#include <memory>
#include <string>
#include <vector>

#include "SDL_include.h"
#include "state/State.h"
#include "util.h"
using std::string;
using std::unique_ptr;
using std::vector;

// constexpr int SCREEN_WIDTH = 1024;
// constexpr int SCREEN_HEIGHT = 600;
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr int FRAME_RATE = 120;
constexpr int64_t FRAME_MS = 1000 / FRAME_RATE;

class Game {
   private:
    // Singletons are just global variables with extra steps
    static Game* instance;
    SDL_Window* window{};
    SDL_Renderer* renderer{};

    vector<unique_ptr<State>> stateStack{};
    vector<State*> stateStackOperations{};  // WARN: nullptr means "pop"
                                            // operation, sorry

    int64_t frameStart{0};
    float dt{0};

    struct {
        float p;
        float duration;
    } slowdown{0, 0};
    float trauma{0};

    Game(const char* title, int width, int height);

    void CalculateDeltaTime();
    void UpdateStateStack();

   public:
    ~Game();

    void Run();
    void RequestPop();
    void RequestPush(State* state);

    int64_t FrameStart();
    float DeltaTime();
    SDL_Renderer* Renderer();
    State& GetState();
    static Game& Instance();

    void Slowdown(float percentage, float durationS);
    void AddTrauma(float delta);
    inline float Trauma() { return trauma; }
};
