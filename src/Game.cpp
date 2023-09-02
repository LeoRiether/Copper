#include "Game.h"
#include "SDL_mixer.h"

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

    renderer = SDL_CreateRenderer(window, -1, 0/* SDL_RENDERER_ACCELERATED */);
    if (!renderer) sdlfail("couldn't create renderer");

    instance->state = new State{};

    log("initialized");
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    warn("destroyed");
}

void Game::Run() {
    log("entering game loop");
    while (!state->QuitRequested()) {
        state->Update(0);
        state->Render();
        SDL_Delay(33);
    }
}

SDL_Renderer* Game::GetRenderer() { return renderer; }

State& Game::GetState() { return *state; }

Game& Game::GetInstance() {
    if (instance == nullptr) {
        // Mankai STEP BY STEP de susume!
        // STEP OUT saa, tobidase!
        instance = new Game{"Leonardo Alves Riether - 190032413", SCREEN_WIDTH,
                            SCREEN_HEIGHT};
    }
    return *instance;
}
