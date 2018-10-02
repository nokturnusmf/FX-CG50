#include <fxcg/system.h>
#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include <string.h>

#include "renderer.h"
#include "game.h"
#include "util.h"

int main() {
    EnableStatusArea(3);
    init_renderer();

    int high = 0;
    while (1) {
        int score = game();
        if (score > high) {
            high = score;
        }

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
}
