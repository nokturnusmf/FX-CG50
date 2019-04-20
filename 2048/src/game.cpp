#include "game.h"

#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/rtc.h>

#include <string.h>

#include "renderer.h"
#include "util.h"
#include "img.h"

struct Animation {
    const unsigned short* sprite;
    int w, h;
    int start_x, start_y;
    int dx, dy;
};

void draw_board() {
    for (int i = 0; i < 198; ++i) {
        for (int j = 0; j < 198; ++j) {
            put_pixel((i % 48 < 6 || j % 48 < 6) ? 0xBD74 : 0xD617, i + 5, j + 13);
        }
    }
}

void draw_tiles(const unsigned char board[16]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[4 * j + i]) {
                draw_sprite(tile_icons[board[4 * j + i] - 1], 48 * i + 11, 48 * j + 19, 42, 42);
            }
        }
    }
}

void draw_scores(int score, int high) {
    char buffer[16] = { ' ', ' ' };

    PrintXY(16, 2, "  Score", 0, 0);
    PrintXY(17, 5, "  High", 0, 0);

    int length = itoa(score, buffer + 2);
    PrintXY(21 - length, 3, buffer, 0, 0);

    length = itoa(high, buffer + 2);
    PrintXY(21 - length, 6, buffer, 0, 0);
}

void draw_animations(Animation* animations, int count, int iter) {
    for (int i = 0; i < iter; ++i) {
        draw_board();
        for (int i = 0; i < count && animations[i].sprite; ++i) {
            draw_sprite(animations[i].sprite,
                        animations[i].start_x += animations[i].dx, animations[i].start_y += animations[i].dy,
                        animations[i].w, animations[i].h);
        }
        write_frame();
    }
}

void draw(const State& state) {
    draw_board();
    draw_tiles(state.board);
    draw_scores(state.score, state.high);

    write_frame();
}

bool finished(const State& state) {
    for (auto c : state.board) {
        if (c == 0) {
            return false;
        }
    }

    for (int i = 0; i < 4; ++i) {
        int a = 0, b = 0;
        for (int j = 0; j < 4; ++j) {
            if (state.board[4 * i + j]) {
                if (state.board[4 * i + j] == a) {
                    return false;
                } else {
                    a = state.board[4 * i + j];
                }
            }

            if (state.board[4 * j + i]) {
                if (state.board[4 * j + i] == b) {
                    return false;
                } else {
                    b = state.board[4 * j + i];
                }
            }
        }
    }

    return true;
}

void reset(State& state) {
    draw(state);

    PrintXY(16, 1, "  Final", 0, 0);
    int t;
    GetKey(&t);
    PrintXY(16, 1, "       ", 0, 0);

    state.score = 0;
    PrintXY(12, 3, "            ", 0, 0);
    memset(state.board, 0, sizeof(state.board));
    state.board[rng_next() % 16] = rng_next() % 4 ? 1 : 2;
}

void reorder(unsigned char board[16], int key) {
    unsigned char board_new[16];

    switch (key) {
    case KEY_CTRL_UP:
        return;

    case KEY_CTRL_DOWN:
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                board_new[4 * r + c] = board[4 * (3 - r) + c];
            }
        }
        break;

    case KEY_CTRL_LEFT:
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                board_new[4 * r + c] = board[4 * (3 - c) + r];
            }
        }
        break;

    case KEY_CTRL_RIGHT:
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                board_new[4 * r + c] = board[4 * c + 3 - r];
            }
        }
        break;
    }

    memcpy(board, board_new, sizeof(board_new));
}

Animation gen_prov_animation(unsigned char tile, int x, int y, int change) {
    return { tile_icons[tile - 1], 42, 42, x, y, 0, change };
}

bool process(State& state, Animation animations[16]) {
    bool last_merged = false;
    bool moved = false;
    int a = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0, n = 0; j < 4; ++j) {
            if (!state.board[4 * j + i]) {
                continue;
            }

            if (n && !last_merged && state.board[4 * j + i] == state.board[4 * (n - 1) + i]) {
                animations[a++] = gen_prov_animation(state.board[4 * j + i], i, j, (n - 1 - j));

                state.score += 1 << ++state.board[4 * (n - 1) + i];
                state.board[4 * j + i] = 0;
                last_merged = true;
                moved = true;
            } else {
                if (j != n) {
                    animations[a++] = gen_prov_animation(state.board[4 * j + i], i, j, (n - j));

                    state.board[4 * n + i] = state.board[4 * j + i];
                    state.board[4 * j + i] = 0;
                    moved = true;
                } else {
                    animations[a++] = gen_prov_animation(state.board[4 * j + i], i, j, 0);
                }
                ++n;
                last_merged = false;
            }
        }
    }
    return moved;
}

void convert_animations(Animation animations[16], int key) {
    static auto x = [](int x_) { return 48 * x_ + 11; };
    static auto y = [](int y_) { return 48 * y_ + 19; };

    int t;
    for (int i = 0; i < 16 && animations[i].sprite; ++i) {
        switch (key) {
        case KEY_CTRL_UP:
            animations[i].start_x = x(animations[i].start_x);
            animations[i].start_y = y(animations[i].start_y);
            animations[i].dy *= 8;
            break;

        case KEY_CTRL_DOWN:
            animations[i].start_x = x(animations[i].start_x);
            animations[i].start_y = y(3 - animations[i].start_y);
            animations[i].dy *= -8;
            break;

        case KEY_CTRL_LEFT:
            t = animations[i].start_x;
            animations[i].start_x = x(animations[i].start_y);
            animations[i].start_y = y(3 - t);
            animations[i].dx = 8 * animations[i].dy;
            animations[i].dy = 0;
            break;

        case KEY_CTRL_RIGHT:
            t = animations[i].start_x;
            animations[i].start_x = x(3 - animations[i].start_y);
            animations[i].start_y = y(t);
            animations[i].dx = -8 * animations[i].dy;
            animations[i].dy = 0;
            break;
        }
    }
}

void update(State& state, int key) {
    if (key == KEY_CTRL_EXIT) {
        reset(state);
        return;
    } else if (key != KEY_CTRL_UP && key != KEY_CTRL_DOWN && key != KEY_CTRL_LEFT && key != KEY_CTRL_RIGHT) {
        return;
    }

    Animation animations[16];
    memset(animations, 0, sizeof(animations));

    reorder(state.board, key);
    auto moved = process(state, animations);

    convert_animations(animations, key);
    if (key == KEY_CTRL_LEFT) key = KEY_CTRL_RIGHT;
    else if (key == KEY_CTRL_RIGHT) key = KEY_CTRL_LEFT;
    reorder(state.board, key);

    draw_animations(animations, 16, 6);

    if (moved) {
        int i;
        while (state.board[i = rng_next() % 16]);
        state.board[i] = rng_next() % 8 ? 1 : 2;
    }

    if (state.score > state.high) {
        state.high = state.score;
    }
}

void game(State& state) {
    unsigned int a, b, c, d;
    RTC_GetTime(&a, &b, &c, &d);
    rng_init(a * b * c * d + a + b + c + d);

    while (1) {
        draw(state);

        int key;
        GetKey(&key);
        update(state, key);

        if (finished(state)) {
            reset(state);
        }
    }
}
