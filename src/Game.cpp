#include "Game.h"

#include <algorithm>

#include "Consts.h"
#include "Resources.h"
#include "SDL_include.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "state/TitleState.h"

#define MODULE "Game"

Game* Game::instance = nullptr;

Game::Game(const char* title, int width, int height) {
    // WARN: prevents the infinite mutual recursion GetInstance -> new Game ->
    // new State -> Sprite::Open -> GetInstance.
    // Jesus Christ.
    instance = this;

    info("initializing...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        sdlfail("couldn't initialize SDL");

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (IMG_Init(img_flags) != img_flags) sdlfail("couldn't initialize images");

    int mix_flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    if (Mix_Init(mix_flags) != mix_flags) sdlfail("couldn't initialize audio");

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                      MIX_DEFAULT_CHANNELS, 1024) != 0)
        sdlfail("couldn't initialize audio (Mix_OpenAudio)");

    if (TTF_Init() != 0) fail2("TTF_Init failed. Cause: %s", TTF_GetError());

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) sdlfail("couldn't create window");

    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) sdlfail("couldn't create renderer");

    frameStart = SDL_GetTicks();

    Consts::Load();

    log("initialized");
}

Game::~Game() {
    stateStack.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    warn("destroyed");
}

void Game::Run() {
    log("entering game loop");
    RequestPush(new TitleState{});
    UpdateStateStack();
    while (!stateStack.empty() && !stateStack.back()->QuitRequested()) {
        auto state = stateStack.back().get();

        trauma *= 0.92;

        auto loopstart = SDL_GetTicks();

        CalculateDeltaTime();
        state->Update(dt);
        auto loopend = SDL_GetTicks();

        if (loopend - loopstart > 10)
            warn2("game update took %dms!", loopend - loopstart);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        state->Render();
        SDL_RenderPresent(renderer);

        UpdateStateStack();

        Consts::PeriodicLoad();

        int64_t deltaMs = (int64_t)SDL_GetTicks() - frameStart;
        SDL_Delay(std::max(0ll, (long long)(FRAME_MS - deltaMs)));
    }
    stateStack.clear();
    Resources::ClearAll();
}

void Game::RequestPop() {
    // nullptr means "pop" stack
    stateStackOperations.emplace_back(nullptr);
}

void Game::RequestPush(State* state) {
    stateStackOperations.emplace_back(state);
}

int64_t Game::FrameStart() { return frameStart; }

float Game::DeltaTime() { return dt; }

SDL_Renderer* Game::Renderer() { return renderer; }

State& Game::GetState() { return *stateStack.back(); }

Game& Game::Instance() {
    if (instance == nullptr) {
        // Mankai STEP BY STEP de susume!
        // STEP OUT saa, tobidase!
        instance = new Game{"COPPER", SCREEN_WIDTH, SCREEN_HEIGHT};
    }
    return *instance;
}

void Game::CalculateDeltaTime() {
    int64_t ticks = SDL_GetTicks();
    dt = float(ticks - frameStart) / 1000.0f;

    if (slowdown.duration > 0) {
        slowdown.duration -= dt;
        dt *= slowdown.p;
    }

    frameStart = ticks;
}

void Game::UpdateStateStack() {
    bool popped = false;
    for (auto state : stateStackOperations) {
        if (state == nullptr) {
            stateStack.pop_back();
            popped = true;
            if (!stateStack.empty()) stateStack.back()->Resume();
        } else {
            if (!stateStack.empty()) stateStack.back()->Pause();
            stateStack.emplace_back(state);
            stateStack.back()->Start();
        }
    }

    if (popped)
        Resources::ClearAll();

    if (!stateStackOperations.empty()) {
        // Resources sometimes takes a long time to load, we
        // should disconsider that time to calculate dt for
        // the next frame
        CalculateDeltaTime();
    }
    stateStackOperations.clear();
}

void Game::Slowdown(float percentage, float durationS) {
    slowdown = {percentage, durationS};
}

void Game::AddTrauma(float x) { trauma = std::min(1.0f, trauma + x); }
