//
// Created by wwz on 2025/10/7.
//

#ifndef MY2048_HYPERPARAMS_H
#define MY2048_HYPERPARAMS_H

// FNN
constexpr double InitLearningRate = 0.001;

// DQN
constexpr int TrainingBatchSize = 32;
constexpr int TrainingFreq = 4;   // 训练周期，隔多少步训练一次
constexpr int ReplayBufferSize = 8192;
constexpr double Gamma = 0.9;
constexpr int MaxStep = 5000;
constexpr int NetworkUpdateFreq = 1000;

// 2048 & DQN
constexpr int GameOverReward = -150;
constexpr int DidntChangeReward = -10;

#endif //MY2048_HYPERPARAMS_H