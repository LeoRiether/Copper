#include <chrono>
#include <iostream>
#include <memory>
#include <random>

#include "Game.h"
#include "SDL_include.h"
#include "util.h"

#define MODULE "main"
int main(int argc, char* argv[]) {
    Game& game = Game::Instance();
    game.Run();
    return EXIT_SUCCESS;
}
