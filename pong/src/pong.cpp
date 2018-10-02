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
    init_sprites();

    int a = 0, b = 0;
    while (1) {        
        if (game()) {
            PrintNumXY(6, 2, ++a, 0x20, TEXT_COLOR_WHITE);
        } else {
            PrintNumXY(13, 2, ++b, 0x20, TEXT_COLOR_WHITE);
        }
        
        Bdisp_PutDisp_DD();
        OS_InnerWait_ms(250);
        int t;
        GetKey(&t);
    }
}
