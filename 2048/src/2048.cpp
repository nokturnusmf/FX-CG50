#include <fxcg/system.h>
#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/file.h>

#include <string.h>

#include "game.h"
#include "renderer.h"

#define UC_LIT(x) (reinterpret_cast<unsigned char*>(const_cast<char*>(x)))

static State state;

void quit_handler() {
    MCSDelVar2(UC_LIT("2048"), UC_LIT("STATE"));
    MCSPutVar2(UC_LIT("2048"), UC_LIT("STATE"), sizeof(state), &state);
}

int main() {
    if (MCS_CreateDirectory(UC_LIT("2048")) == 0x40) {
        int len;
        MCSGetDlen2(UC_LIT("2048"), UC_LIT("STATE"), &len);
        MCSGetData1(0, sizeof(state), &state);
    } else {
        memset(&state, 0, sizeof(state));
        state.board[0] = 1;
    }

    SetQuitHandler(quit_handler);

    EnableStatusArea(3);
    init_renderer();
    clear();

    game(state);
}
