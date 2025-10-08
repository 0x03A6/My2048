//
// Created by wwz on 2025/9/27.
//

#ifndef MY2048_GAME2048_H
#define MY2048_GAME2048_H
#include <random>

enum class Direction : unsigned char { Up, Down, Left, Right, Invalid };

struct ActionEffect {
    int delta_score;
    bool didnt_change;
    bool game_over;
};

class Game2048 {
    int board[4][4];
    int space_cnt;
    int score;
    bool generateRandElem(std::mt19937 &rng);
    bool shiftLine(int &a, int &b, int &c, int &d);

    bool updateLine(int &delta_score, int &a, int &b, int &c, int &d);
    static bool judgeFullLineAlive(int a, int b, int c, int d);
    bool judgeFullBoardAlive() const;
public:
    Game2048();
    Game2048(std::mt19937 &rng);
    Game2048(const Game2048&);
    ActionEffect update(Direction d, std::mt19937 &rng);
    void print();
    void play(std::mt19937 &rng);
    const int *getBoard() const;
    double *getRealBoard() const;
    int getScore() const;
    void reset(std::mt19937 &rng);
};


#endif //MY2048_GAME2048_H