#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "Layer.h"
#include "FNN.h"
#include "DQN.h"

int main() {

    DQN dqn;
    dqn.train();

    return 0;
}