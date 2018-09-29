#include "util.h"

#include <fxcg/display.h>

void itoa(long num, char* str, int base) {
    if (num < 0) {
        *(str++) = '-';
        num = -num;
    } else if (num == 0) {
        str[0] = '0';
        return;
    }

    const char* base_string = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    str[0] = '\0';
    for (int i = 0; num; ++i, num /= base) {
        for (int j = i; j >= 0; --j) {
            str[j + 1] = str[j];
        }
        str[0] = base_string[num % base];
    }
}

void PrintNumXY(int x, int y, int i, int mode, int color) {
    char str[10] = { ' ', ' ' };
    itoa(i, str + 2);
    PrintXY(x, y, str, mode, color);
}

static unsigned int rng_state;

void rng_init(unsigned int seed) {
    rng_state = seed;
}

unsigned int rng_next() {
    rng_state ^= rng_state << 13;
	rng_state ^= rng_state >> 17;
	rng_state ^= rng_state << 5;
    return rng_state;
}
