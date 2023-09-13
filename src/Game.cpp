#include "Game.h"

#include "Resources.h"
#include "SDL_mixer.h"
#include "SDL_timer.h"
#include "StageState.h"
#include "TitleState.h"

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

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) sdlfail("couldn't create window");

    renderer = SDL_CreateRenderer(window, -1, 0 /* SDL_RENDERER_ACCELERATED */);
    if (!renderer) sdlfail("couldn't create renderer");

    instance->stateStack.clear();
    Push(new TitleState{});

    frameStart = SDL_GetTicks();

    log("initialized");
}

Game::~Game() {
    stateStack.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    warn("destroyed");
}

void Game::CalculateDeltaTime() {
    uint32_t ticks = SDL_GetTicks();
    dt = float(ticks - frameStart) / 1000.0f;
    frameStart = ticks;
}

void Game::Run() {
    log("entering game loop");
    while (!stateStack.empty() && !stateStack.back()->QuitRequested()) {
        auto state = stateStack.back().get();

        CalculateDeltaTime();
        state->Update(dt);

        state->Render();
        SDL_RenderPresent(renderer);

        if (state->PopRequested()) {
            stateStack.pop_back();
            if (!stateStack.empty()) stateStack.back()->Resume();
        }

        SDL_Delay(33);
    }
    Resources::ClearImages();
    Resources::ClearMusic();
    Resources::ClearSounds();
}

void Game::Push(State* state) {
    if (!stateStack.empty()) stateStack.back()->Pause();
    stateStack.emplace_back(state);
    stateStack.back()->Start();
}

float Game::DeltaTime() { return dt; }

SDL_Renderer* Game::Renderer() { return renderer; }

State& Game::GetState() { return *stateStack.back(); }

Game& Game::Instance() {
    if (instance == nullptr) {
        // Mankai STEP BY STEP de susume!
        // STEP OUT saa, tobidase!
        instance = new Game{"Leonardo Alves Riether - 190032413", SCREEN_WIDTH,
                            SCREEN_HEIGHT};
    }
    return *instance;
}
