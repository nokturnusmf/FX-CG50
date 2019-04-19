#ifndef GAME_H
#define GAME_H

struct State {
    int score;
    int high;

    unsigned char board[4 * 4];
};

void game(State& state);

#endif // GAME_H
