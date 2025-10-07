//
// Created by wwz on 2025/10/7.
//

#ifndef MY2048_DQN_H
#define MY2048_DQN_H

#include "FNN.h"
#include "ReplayBuffer.h"

class DQN {
    FNN policy_network;
    FNN target_network;
    ReplayBuffer<ReplayBufferSize> replay_buffer;
    Game2048 game;
    int step = 0;

public:
    void learn(const TrainingData &data);
    void setLR();
    void train();

};


#endif //MY2048_DQN_H