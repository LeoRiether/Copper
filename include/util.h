#pragma once
// "util.h é a caixinha da bagunça"
//     – cara do meu estágio

#include <chrono>
#include <random>

#define BASEDIR "./"
#define ASSETS "assets"

#define RESET "\033[0m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define CYAN "\033[96m"
#define WHITE "\033[97m"

#define log(fmt) \
    { fprintf(stderr, BLUE MODULE ":: " RESET fmt "\n"); }
#define log2(fmt, ...) \
    { fprintf(stderr, BLUE MODULE ":: " RESET fmt "\n", __VA_ARGS__); }
#define info(fmt) \
    { fprintf(stderr, GREEN MODULE ":: " RESET fmt "\n"); }
#define info2(fmt, ...) \
    { fprintf(stderr, GREEN MODULE ":: " RESET fmt "\n", __VA_ARGS__); }
#define debug(fmt) \
    { fprintf(stderr, CYAN MODULE ":: " RESET fmt "\n"); }
#define debug2(fmt, ...) \
    { fprintf(stderr, CYAN MODULE ":: " RESET fmt "\n", __VA_ARGS__); }
#define warn(fmt) \
    { fprintf(stderr, YELLOW MODULE ":: " RESET fmt "\n"); }
#define warn2(fmt, ...) \
    { fprintf(stderr, YELLOW MODULE ":: " RESET fmt "\n", __VA_ARGS__); }
#define error(fmt) \
    { fprintf(stderr, RED MODULE ":: " RESET fmt "\n"); }

#define fail(fmt)                                                      \
    {                                                                  \
        fprintf(stderr, RED MODULE ":: Fatal error: " RESET fmt "\n"); \
        exit(EXIT_FAILURE);                                            \
    }
#define fail2(fmt, ...)                                               \
    {                                                                 \
        fprintf(stderr, RED MODULE ":: Fatal error: " RESET fmt "\n", \
                __VA_ARGS__);                                         \
        exit(EXIT_FAILURE);                                           \
    }
#define sdlfail(fmt)                                                         \
    {                                                                        \
        fprintf(stderr,                                                      \
                RED MODULE ":: Fatal SDL error: " RESET fmt ". Cause: %s\n", \
                SDL_GetError());                                             \
        exit(EXIT_FAILURE);                                                  \
    }

#define UNUSED(x) \
    { (void)(x); }

inline std::mt19937 rng(
    std::chrono::steady_clock::now().time_since_epoch().count());

inline float randf(float low, float high) {
    auto dist = std::uniform_real_distribution<float>(low, high);
    return dist(rng);
}

// [low, high]
inline int randi(int low, int high) {
    auto dist = std::uniform_int_distribution<int>(low, high);
    return dist(rng);
}
