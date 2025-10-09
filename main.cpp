#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "Layer.h"
#include "FNN.h"
#include "DQN.h"

int main() {

    DQN dqn;

    for (int i = MaxStep; i < 100 * MaxStep; i += MaxStep) {
        dqn.train(i);
        dqn.evaluate();
        getchar();
    }
    return 0;
}