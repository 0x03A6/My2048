//
// Created by wwz on 2025/9/27.
//

#include "Game2048.h"

#include <random>
#include <cstring>
#include <filesystem>

#include "print.h"
#include "utils.h"

bool Game2048::generateRandElem(std::mt19937 &rng) {
    if (space_cnt == 0)
        return false;
    const int idx = static_cast<int>(rng() % static_cast<unsigned int>(space_cnt));
    for (int i = 0, j = 0; i < 16; i++) {
        if ((reinterpret_cast<int *>(board))[i] == 0) {
            if (j == idx) {
                (reinterpret_cast<int *>(board))[i] = !(bool)(rng() % 10) + 1;
                space_cnt--;
                return true;
            }
            j++;
        }
    }
    return false;
}

bool Game2048::shiftLine(int &a, int &b, int &c, int &d) {
    bool moved = false;
    if (c == 0 && d != 0) {
        c = d;
        d = 0;
        moved = true;
    }
    if (b == 0 && c != 0) {
        b = c;
        c = d;
        d = 0;
        moved = true;
    }
    if (a == 0 && b != 0) {
        a = b;
        b = c;
        c = d;
        d = 0;
        moved = true;
    }
    return moved;
}

bool Game2048::updateLine(int &delta_score, int &a, int &b, int &c, int &d) {
    if (a != 0) {
        if (a == b) {
            delta_score += 1 << a;
            a++;
            b = 0;
            space_cnt++;
            if (c != 0 && c == d) {
                delta_score += 1 << c;
                c++;
                d = 0;
                space_cnt++;
            }
            goto SHIFT;
        }
        if (a == c && b == 0) {
            delta_score += 1 << a;
            a++;
            c = 0;
            space_cnt++;
            goto SHIFT;
        }
        if (a == d && b == 0 && c == 0) {
            delta_score += 1 << a;
            a++;
            d = 0;
            space_cnt++;
            goto SHIFT;
        }
    }
    if (b != 0) {
        if (b == c) {
            delta_score += 1 << b;
            b++;
            c = 0;
            space_cnt++;
            goto SHIFT;
        }
        if (b == d && c == 0) {
            delta_score += 1 << b;
            b++;
            d = 0;
            space_cnt++;
            goto SHIFT;
        }
    }
    if (c != 0 && c == d) {
        delta_score += 1 << c;
        c++;
        d = 0;
        space_cnt++;
    }
    SHIFT:
    return shiftLine(a, b, c, d) || delta_score;
}

bool Game2048::judgeFullLineAlive(const int a, const int b, const int c, const int d) {
    return a == b || b == c || c == d;
}

bool Game2048::judgeFullBoardAlive() const {
#define JUDGE_LINE_ALIVE_HELPER(x, a, b, c, d) if (judgeFullLineAlive(board[x][a], board[x][b], board[x][c], board[x][d])) return true
#define JUDGE_COLUMN_ALIVE_HELPER(x, a, b, c, d) if (judgeFullLineAlive(board[a][x], board[b][x], board[c][x], board[d][x])) return true

    JUDGE_COLUMN_ALIVE_HELPER(0, 0, 1, 2, 3);
    JUDGE_COLUMN_ALIVE_HELPER(1, 0, 1, 2, 3);
    JUDGE_COLUMN_ALIVE_HELPER(2, 0, 1, 2, 3);
    JUDGE_COLUMN_ALIVE_HELPER(3, 0, 1, 2, 3);

    JUDGE_COLUMN_ALIVE_HELPER(0, 3, 2, 1, 0);
    JUDGE_COLUMN_ALIVE_HELPER(1, 3, 2, 1, 0);
    JUDGE_COLUMN_ALIVE_HELPER(2, 3, 2, 1, 0);
    JUDGE_COLUMN_ALIVE_HELPER(3, 3, 2, 1, 0);

    JUDGE_LINE_ALIVE_HELPER(0, 0, 1, 2, 3);
    JUDGE_LINE_ALIVE_HELPER(1, 0, 1, 2, 3);
    JUDGE_LINE_ALIVE_HELPER(2, 0, 1, 2, 3);
    JUDGE_LINE_ALIVE_HELPER(3, 0, 1, 2, 3);

    JUDGE_LINE_ALIVE_HELPER(0, 3, 2, 1, 0);
    JUDGE_LINE_ALIVE_HELPER(1, 3, 2, 1, 0);
    JUDGE_LINE_ALIVE_HELPER(2, 3, 2, 1, 0);
    JUDGE_LINE_ALIVE_HELPER(3, 3, 2, 1, 0);

#undef JUDGE_LINE_ALIVE_HELPER
#undef JUDGE_COLUMN_ALIVE_HELPER

    return false;
}

Game2048::Game2048() {
    memset(board, 0, sizeof(board));
    space_cnt = 16;
    score = 0;
    generateRandElem(getRng());
    generateRandElem(getRng());
}

Game2048::Game2048(const Game2048 &game) {
    memcpy(board, game.board, sizeof(board));
    space_cnt = game.space_cnt;
    score = game.score;
}

int Game2048::update(const Direction d, std::mt19937 &rng) {
    bool moved = false;
    int delta_score;
    int sum_delta_score = 0;
#define UPDATE_LINE_HELPER(x, a, b, c, d) delta_score = 0, moved |= updateLine(delta_score, board[x][a], board[x][b], board[x][c], board[x][d]), sum_delta_score += delta_score
#define UPDATE_COLUMN_HELPER(x, a, b, c, d) delta_score = 0, moved |= updateLine(delta_score, board[a][x], board[b][x], board[c][x], board[d][x]), sum_delta_score += delta_score
    switch (d) {
        case Direction::Up:
            UPDATE_COLUMN_HELPER(0, 0, 1, 2, 3);
            UPDATE_COLUMN_HELPER(1, 0, 1, 2, 3);
            UPDATE_COLUMN_HELPER(2, 0, 1, 2, 3);
            UPDATE_COLUMN_HELPER(3, 0, 1, 2, 3);
            break;
        case Direction::Down:
            UPDATE_COLUMN_HELPER(0, 3, 2, 1, 0);
            UPDATE_COLUMN_HELPER(1, 3, 2, 1, 0);
            UPDATE_COLUMN_HELPER(2, 3, 2, 1, 0);
            UPDATE_COLUMN_HELPER(3, 3, 2, 1, 0);
            break;
        case Direction::Left:
            UPDATE_LINE_HELPER(0, 0, 1, 2, 3);
            UPDATE_LINE_HELPER(1, 0, 1, 2, 3);
            UPDATE_LINE_HELPER(2, 0, 1, 2, 3);
            UPDATE_LINE_HELPER(3, 0, 1, 2, 3);
            break;
        case Direction::Right:
            UPDATE_LINE_HELPER(0, 3, 2, 1, 0);
            UPDATE_LINE_HELPER(1, 3, 2, 1, 0);
            UPDATE_LINE_HELPER(2, 3, 2, 1, 0);
            UPDATE_LINE_HELPER(3, 3, 2, 1, 0);
    }
#undef UPDATE_LINE_HELPER
#undef UPDATE_COLUMN_HELPER
    score += sum_delta_score;
    if (moved)
        generateRandElem(rng);
    return sum_delta_score;
}

void Game2048::print() {
    static auto printer = parseStr<int, int, int, int>("\t{}\t{}\t{}\t{}\n\n");
    fillValue(printer, board[0][0], board[0][1], board[0][2], board[0][3]);
    printTuple(printer);
    fillValue(printer, board[1][0], board[1][1], board[1][2], board[1][3]);
    printTuple(printer);
    fillValue(printer, board[2][0], board[2][1], board[2][2], board[2][3]);
    printTuple(printer);
    fillValue(printer, board[3][0], board[3][1], board[3][2], board[3][3]);
    printTuple(printer);
}

void Game2048::play(std::mt19937 &rng) {
    print();
    fflush(stdout);
    while (true) {
        int c = getchar();
        int reward;
        switch (c) {
            case 'w':
                reward = update(Direction::Up, rng);
                break;
            case 'a':
                reward = update(Direction::Left, rng);
                break;
            case 's':
                reward = update(Direction::Down, rng);
                break;
            case 'd':
                reward = update(Direction::Right, rng);
                break;
            default:
                continue;
        }
        print();
        std::cout << "Reward: " << reward << "\nScore: " << score << '\n';
        fflush(stdout);
        if (space_cnt == 0 && !judgeFullBoardAlive()) {
            std::cout << "Game Over.\n";
            break;
        }
    }
}

int * Game2048::getBoard() {
    return (int *)board;
}

double * Game2048::getRealBoard() {
    static double ret[16];
    for (int i = 0; i < 16; i++)
        ret[i] = ((int *)board)[i];
    return ret;
}
