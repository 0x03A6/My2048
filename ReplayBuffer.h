//
// Created by wwz on 2025/10/7.
//

#ifndef MY2048_REPLAYBUFFER_H
#define MY2048_REPLAYBUFFER_H

#include "Game2048.h"

struct TrainingData {
    int board[4][4];        // S
    Direction action;       // A
    int reward;             // R
    int nxt_board[4][4];    // S'
};

template<int N>
class ReplayBuffer {
    TrainingData *buf;
    int curs;
public:
    ReplayBuffer() {
        buf = new TrainingData[N];
        for (int i = 0; i < N; i++) {
            buf[i].action = Direction::Invalid;
        }
        curs = 0;
    }
    void push_back(const TrainingData& data) {
        if (curs <= N) {
            buf[curs] = data;
            curs++;
        } else {
            curs = 0;
            buf[0] = data;
        }
    }
    [[nodiscard]] TrainingData &back() const {
        return buf[curs];
    }

    void addEmptyElem() {
        curs++;
    }
    TrainingData &pick(std::mt19937 &rng) const {
        std::uniform_int_distribution<> dis(0, N - 1);
        return buf[dis(rng)];
    }
};


#endif //MY2048_REPLAYBUFFER_H