#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <random>

using std::shared_ptr;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

#define fail(fmt)                                                 \
    {                                                             \
        fprintf(stderr, "\033[91mFatal error: \033[0m" fmt "\n"); \
        exit(EXIT_FAILURE);                                       \
    }
#define fail2(fmt, ...)                                                        \
    {                                                                          \
        fprintf(stderr, "\033[91mFatal error: \033[0m" fmt "\n", __VA_ARGS__); \
        exit(EXIT_FAILURE);                                                    \
    }

struct Texture {
    SDL_Texture* sdl;
    uint32_t id = rng();

    ~Texture() {
        std::cout << "Destroyed texture " << id << std::endl;
        SDL_DestroyTexture(sdl);
    }
};

template <typename X, typename Y>
struct Pair {
    X x;
    Y y;
};

// x, y: upper left corner.
Pair<shared_ptr<Texture>, SDL_Rect> get_text_and_rect(SDL_Renderer* renderer,
                                                      int x, int y, char* text,
                                                      TTF_Font* font) {
    int text_width;
    int text_height;
    SDL_Surface* surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = text_width;
    rect.h = text_height;

    return {shared_ptr<Texture>(new Texture{texture}), rect};
}

int main(int argc, char* argv[]) {
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;
    char* font_path;
    int quit;

    if (argc == 1) {
        // font_path = (char*)"SpicySoup.ttf";
        font_path = (char*)"penguin/assets/font/Call me maybe.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fail("too many arguments");
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO)) {
        fail("Failed to initialize SDL");
    }

    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
                                    SDL_RENDERER_ACCELERATED, &window,
                                    &renderer)) {
        fail("Failed to create window and renderer\n");
    }

    TTF_Init();
    TTF_Font* font = TTF_OpenFont(font_path, 64);
    if (font == NULL) {
        fail("Font not found\n");
    }
    auto [texture1_aux, rect1_aux] = get_text_and_rect(
        renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (char*)"hello", font);
    auto [texture1, rect1] = get_text_and_rect(
        renderer, SCREEN_WIDTH / 2 - rect1_aux.w / 2, SCREEN_HEIGHT / 2 - rect1_aux.h, (char*)"hello", font);
    auto [texture2, rect2] = get_text_and_rect(
        renderer, rect1.x, rect1.y + rect1.h, (char*)"world", font);

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        /* Use TTF textures. */
        SDL_RenderCopy(renderer, texture1->sdl, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2->sdl, NULL, &rect2);

        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
