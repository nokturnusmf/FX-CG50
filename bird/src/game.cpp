#include "game.h"

#include <fxcg/system.h>
#include <fxcg/rtc.h>
#include <string.h>

#include "renderer.h"
#include "img.h"
#include "input.h"
#include "util.h"

#define SCREEN_TOP 8
#define SCREEN_BOT 190
#define SEP_V 40
#define SEP_H 96
#define PIPE_COUNT 5
#define BIRD_X 80
#define SCROLL_SPEED 4

void draw_pipe(int x, int y) {
    for (int i = SCREEN_TOP; i < y - SEP_V; ++i) {
        draw_sprite(pipe_upper, x, i, PIPE_W, 1);
    }
    draw_sprite(pipe_upper, x, y - SEP_V, PIPE_W, PIPE_H);

    draw_sprite(pipe_lower, x, y + SEP_V, PIPE_W, PIPE_H);
    for (int i = y + SEP_V + PIPE_H; i < SCREEN_BOT; ++i) {
        draw_sprite(pipe_lower + PIPE_W * (PIPE_H - 1), x, i, PIPE_W, 1);
    }
}

void draw_ground() {
    for (int i = 0; i < LCD_WIDTH_PX / GROUND_W; ++i) {
        draw_sprite(ground, i * GROUND_W, SCREEN_BOT, GROUND_W, GROUND_H);
        draw_sprite(ground + 0x228, i * GROUND_W, 214, GROUND_W, 1);
        draw_sprite(ground + 0x228, i * GROUND_W, 215, GROUND_W, 1);
    }
}

void render(int bird_h, int offset, int* pipes) {
    clear(0xffff, 0, SCREEN_TOP);
    clear(0x7edf, SCREEN_TOP, SCREEN_BOT);

    for (int i = 0; i < PIPE_COUNT; ++i) {
        if (pipes[i]) {
            draw_pipe(offset + i * SEP_H, pipes[i]);
        }
    }

    draw_sprite(bird, BIRD_X, bird_h, BIRD_W, BIRD_H);
}

inline int new_pipe() {
    return SCREEN_TOP + PIPE_H + SEP_V + (rng_next() % (SCREEN_BOT - 2 * (PIPE_H + SEP_V) - SCREEN_TOP));
}

inline bool overlap(int a, int a_d, int b, int b_d) {
    return a < b ? a + a_d > b : a < b + b_d;
}

inline bool within(int a, int a_d, int b, int b_d) {
    return a > b && a + a_d < b + b_d;
}

bool dead(int bird_h, int offset, int pipe0, int pipe1) {
    if (pipe0 && overlap(BIRD_X, BIRD_W, offset, PIPE_W)) {
        return !within(bird_h, BIRD_H, pipe0 - SEP_V + PIPE_H, SEP_V * 2 - PIPE_H);
    } else if (pipe1 && overlap(BIRD_X, BIRD_W, offset + SEP_H, PIPE_W)) {
        return !within(bird_h, BIRD_H, pipe1 - SEP_V + PIPE_H, SEP_V * 2 - PIPE_H);
    }

    return bird_h >= SCREEN_BOT;
}

int game() {
    unsigned int a, b, c, d;
    RTC_GetTime(&a, &b, &c, &d);
    rng_init(a * b * c * d + a + b + c + d);

    draw_ground();

    int score = -4;

    int pipes[PIPE_COUNT] = { 0 };
    int offset = 0;

    int bird_h = 80;
    int bird_v = 0;

    while (!dead(bird_h, offset, pipes[0], pipes[1])) {
        if (key_update() && bird_h > 30) {
            bird_v = -6;
        }
        bird_h += ++bird_v;

        render(min(SCREEN_BOT - BIRD_H, bird_h), offset, pipes);

        if ((offset -= SCROLL_SPEED) <= -PIPE_W) {
            memmove(pipes, pipes + 1, (PIPE_COUNT - 1) * sizeof(int));
            pipes[PIPE_COUNT - 1] = new_pipe();
            offset += SEP_H;
            ++score;
        }
        PrintNumXY(1, 1, max(0, score), 0x20, TEXT_COLOR_WHITE);

        write_frame();
        OS_InnerWait_ms(16);
    }

    bird_v = 0;
    while ((bird_h += ++bird_v) <= SCREEN_BOT - BIRD_H) {
        render(bird_h, offset, pipes);
        write_frame();
    }

    render(SCREEN_BOT - BIRD_H, offset, pipes);

    return max(0, score);
}
