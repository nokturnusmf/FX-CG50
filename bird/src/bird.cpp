#include <fxcg/system.h>
#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/file.h>

#include <string.h>

#include "renderer.h"
#include "game.h"
#include "util.h"

#define UC_LIT(x) (reinterpret_cast<unsigned char*>(const_cast<char*>(x)))

void display(int score, int high) {
    MsgBoxPush(2);

    char score_str[20] = "  Score: ";
    char high_str[20] = "  High: ";

    itoa(score, score_str + 9);
    itoa(high, high_str + 8);

    PrintXY(6, 3, score_str, 0, TEXT_COLOR_BLACK);
    PrintXY(7, 4, high_str, 0, TEXT_COLOR_BLACK);

    int t;
    GetKey(&t);
    MsgBoxPop();
}

static int high = 0;

void quit_handler() {
    MCSDelVar2(UC_LIT("BIRD"), UC_LIT("SCORE"));
    MCSPutVar2(UC_LIT("BIRD"), UC_LIT("SCORE"), 4, &high);
}

int main() {
    if (MCS_CreateDirectory(UC_LIT("2048")) == 0x42) {
        int len;
        MCSGetDlen2(UC_LIT("BIRD"), UC_LIT("SCORE"), &len);
        MCSGetData1(0, 4, &high);
    }
    
    SetQuitHandler(quit_handler);

    EnableStatusArea(3);
    init_renderer();

    while (1) {
        int score = game();
        if (score > high) {
            high = score;
        }

        display(score, high);
    }
}
