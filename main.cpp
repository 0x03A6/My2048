#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "Layer.h"
#include "FNN.h"
#include "DQN.h"

int main() {

    Game2048 game;
    game.play(getRng());

    DQN dqn;
    return 0;
}