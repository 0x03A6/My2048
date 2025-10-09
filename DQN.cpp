//
// Created by wwz on 2025/10/7.
//

#include "DQN.h"

#include "print.h"

/// @attention 注意梯度设置的正负性，可能bug
void DQN::learn(const TrainingData &data) {
    if (data.action == Direction::Invalid)
        return;
    std::cout << "Learned.\n";
    policy_network.setInput((int *)data.board);
    const auto res_policy = policy_network.forward();
    target_network.setInput((int *)data.nxt_board);
    const auto res_target = target_network.forward();
    const double Q = data.reward + Gamma * std::max({ res_target[0], res_target[1], res_target[2], res_target[3] });
    static double t[4];
    static Vector<double, 4> grad(t);
    grad[0] = grad[1] = grad[2] = grad[3] = 0;
    grad[(int)data.action] = res_policy[(int)data.action] - Q;
    policy_network.backward(grad);
}

void DQN::setLR() {
    if (step == 0)
        policy_network.setLearningRate(InitLearningRate);
    else if (step == 5000)
        policy_network.setLearningRate(InitLearningRate * 0.1);
    else if (step == 20000)
        policy_network.setLearningRate(InitLearningRate * 0.01);
}

void DQN::train(const int max_step) {
    game.reset(getRng());
    for (; step < max_step; step++) {
        setLR();
        if (step % TrainingFreq == 0) {
            for (int i = 0; i < TrainingBatchSize; i++) {
                learn(replay_buffer.pick(getRng()));
            }
        }
        policy_network.setInput(game.getBoard());
        auto res = policy_network.forward();
        const double max_q = std::max({ res[0], res[1], res[2], res[3] });

        std::cout << "res: [ U: " << res[0] << " D: " << res[1] << " L: " << res[2] << " R: " << res[3] << " ]\n";

        int decision;
        std::bernoulli_distribution dis;
        if (dis(getRng())) {
            if (res[0] == max_q)        decision = 0;
            else if (res[1] == max_q)   decision = 1;
            else if (res[2] == max_q)   decision = 2;
            else                        decision = 3;
        } else {
            std::uniform_int_distribution<int> rand_direction(0, 3);
            decision = rand_direction(getRng());
        }

        std::cout << "Decision: " << "UDLR"[decision] << std::endl;

        replay_buffer.addEmptyElem();
        replay_buffer.back().action = static_cast<Direction>(decision);
        memcpy(replay_buffer.back().board, game.getBoard(), 16 * sizeof(int));

        const auto [delta_score, didnt_change, game_over] = game.update(static_cast<Direction>(decision), getRng());

        memcpy(replay_buffer.back().nxt_board, game.getBoard(), 16 * sizeof(int));

        replay_buffer.back().reward = delta_score;
        if (game_over)
            replay_buffer.back().reward += GameOverReward;
        if (didnt_change)
            replay_buffer.back().reward += DidntChangeReward;

        std::cout << "Reward: " << replay_buffer.back().reward << std::endl;

        game.print();
        std::cout << "Finished Step: " << step << "\nScore: " << game.getScore() << "\n\n";

        if (game_over || didnt_change)
            game.reset(getRng());

        if (step % NetworkUpdateFreq == 0) {
            target_network.copy(policy_network);
            std::cout << "Network updated.\n";
        }
    }
}

void DQN::evaluate() {
    std::cout << "Evaluation.\n";
    int s = 0;
    for (int i = 0; i < 5; i++) {
        game.reset(getRng());
        ActionEffect effect;
        while (true) {
            policy_network.setInput(game.getBoard());
            const auto res_policy = policy_network.forward();
            const int action = std::max_element(res_policy.getData(), res_policy.getData() + 4) - res_policy.getData();
            std::cout << "UDLR"[action];
            effect = game.update(static_cast<Direction>(action), getRng());
            if (effect.game_over) {
                std::cout << "\nGame over.\n";
                break;
            }
            if (effect.didnt_change) {
                std::cout << "\nDidnt change.\n";
                break;
            }
        }
        std::cout << "\nScore: " << game.getScore() << std::endl;
        s += game.getScore();
    }
    std::cout << "Average Score: " << s / 5.0 << std::endl;
}
