#include <fxcg/display.h>
#include <fxcg/keyboard.h>

int main() {
    PrintXY(1, 1, "  Example", 0, TEXT_COLOR_BLACK);
    int key;
	while (1) {
        GetKey(&key);
	}
}

