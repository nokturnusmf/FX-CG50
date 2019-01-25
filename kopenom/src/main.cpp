#include <fxcg/system.h>
#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include <string.h>

#include "renderer.h"

int main() {
    EnableStatusArea(3);
    init_renderer();

    while (1) {
        int t;
        GetKey(&t);
    }
}
