#include "game.h"

#include <fxcg/system.h>
#include <fxcg/rtc.h>

#include <string.h>

#include "renderer.h"
#include "input.h"
#include "util.h"

#define SCREEN_TOP 8
#define SCREEN_BOT 216

#define HORIZONTAL_V 6
#define VERTICAL_VMAX 10
#define PADDLE_SPEED 4

#define BALL_SIZE 32
#define PADDLE_H 48
#define PADDLE_W 16

unsigned short ball[BALL_SIZE * BALL_SIZE];
unsigned short paddle[PADDLE_W * PADDLE_H];

void init_sprites() {
    memset(ball, 0xff, sizeof(ball));
    memset(paddle, 0xff, sizeof(paddle));
}

void render(int a_h, int b_h, int ball_x, int ball_y) {
    clear(0x0821, SCREEN_TOP, SCREEN_BOT);

    draw_sprite(ball, ball_x, ball_y, BALL_SIZE, BALL_SIZE);
    
    draw_sprite(paddle, 0, a_h, PADDLE_W, PADDLE_H);
    draw_sprite(paddle, LCD_WIDTH_PX - PADDLE_W, b_h, PADDLE_W, PADDLE_H);
}

inline int abs(int x) {
    return x > 0 ? x : -x;
}

inline int clamp(int x, int l, int u) {
    return x < l ? l : x > u ? u : x;
}

inline bool overlap(int a, int a_d, int b, int b_d) {
    return a < b ? a + a_d > b : a < b + b_d;
}

bool game() {
    unsigned int a, b, c, d;
    RTC_GetTime(&a, &b, &c, &d);
    
    int ball_x = LCD_WIDTH_PX / 2, ball_y = LCD_HEIGHT_PX / 2;
    int ball_vx = d & 1 ? HORIZONTAL_V : -HORIZONTAL_V;
    int ball_vy = (a * b * c * d + a + b + c + d) % VERTICAL_VMAX + 1; // TODO

    int a_h = LCD_HEIGHT_PX / 2, b_h = LCD_HEIGHT_PX / 2;

    while (1) {
        key_update();
        
        if (is_key_pressed(KEY_PRGM_ACON) || is_key_pressed(KEY_PRGM_EXIT)) {
            return a & 0b00100100;
        }
        
        a_h += PADDLE_SPEED * (is_key_pressed(KEY_PRGM_1) - is_key_pressed(KEY_PRGM_7));
        a_h = clamp(a_h, SCREEN_TOP, SCREEN_BOT - PADDLE_H);
        b_h += PADDLE_SPEED * (is_key_pressed(KEY_PRGM_3) - is_key_pressed(KEY_PRGM_9));
        b_h = clamp(b_h, SCREEN_TOP, SCREEN_BOT - PADDLE_H);
        
        if (ball_y <= SCREEN_TOP || ball_y >= SCREEN_BOT - BALL_SIZE) {
            ball_vy = -ball_vy;
        }
        
        if (ball_x < PADDLE_W) {
            if (ball_x < 0) {
                return false;
            } else if (overlap(a_h, PADDLE_H, ball_y, BALL_SIZE)) {
                ball_vx = abs(ball_vx);
            }
        }
        
        if (ball_x > LCD_WIDTH_PX - PADDLE_W - BALL_SIZE) {
            if (ball_x > LCD_WIDTH_PX - BALL_SIZE) {
                return true;
            } else if (overlap(b_h, PADDLE_H, ball_y, BALL_SIZE)) {
                ball_vx = -abs(ball_vx);
            }
        }

        render(a_h, b_h, ball_x += ball_vx, ball_y += ball_vy);
        
        write_frame();
        OS_InnerWait_ms(16);
    }
}
