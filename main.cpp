#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "utils.h"

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());
    Game2048 game(rng);
    game.play(rng);
    return 0;
    return 0;
}