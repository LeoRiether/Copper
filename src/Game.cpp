#include "Game.h"

#include "InputManager.h"
#include "Resources.h"
#include "SDL_include.h"
#include "state/TitleState.h"

#define MODULE "Game"

Game* Game::instance = nullptr;

Game::Game(const char* title, int width, int height) {
    // WARN: prevents the infinite mutual recursion GetInstance -> new Game ->
    // new State -> Sprite::Open -> GetInstance.
    // Jesus Christ.
    instance = this;

    info("initializing...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
        sdlfail("couldn't initialize SDL");

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
    if (IMG_Init(img_flags) != img_flags) sdlfail("couldn't initialize images");

    int mix_flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_OPUS;
    if (Mix_Init(mix_flags) != mix_flags) sdlfail("couldn't initialize audio");

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                      MIX_DEFAULT_CHANNELS, 1024) != 0)
        sdlfail("couldn't initialize audio (Mix_OpenAudio)");

    if (TTF_Init() != 0) fail2("TTF_Init failed. Cause: %s", TTF_GetError());

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) sdlfail("couldn't create window");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) sdlfail("couldn't create renderer");

    frameStart = SDL_GetTicks();

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

        CalculateDeltaTime();
        state->Update(dt);

        SDL_RenderClear(renderer);
        state->Render();
        SDL_RenderPresent(renderer);

        UpdateStateStack();

        int64_t deltaMs = (int64_t)SDL_GetTicks64() - frameStart;
        SDL_Delay(std::max(0l, FRAME_MS - deltaMs));
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
    int64_t ticks = SDL_GetTicks64();
    dt = float(ticks - frameStart) / 1000.0f;
    frameStart = ticks;
}

void Game::UpdateStateStack() {
    for (auto state : stateStackOperations) {
        if (state == nullptr) {
            stateStack.pop_back();
            Resources::ClearAll();
            if (!stateStack.empty()) stateStack.back()->Resume();
        } else {
            if (!stateStack.empty()) stateStack.back()->Pause();
            stateStack.emplace_back(state);
            stateStack.back()->Start();
        }
    }
    stateStackOperations.clear();
}
