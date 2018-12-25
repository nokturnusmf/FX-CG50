#include "input.h"

#include <string.h>

static const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;

static unsigned short cur[8];
static unsigned short prev[8];

bool key_update() {
    bool flag = false;

    for (int i = 0; i < 8; ++i) {
        prev[i] = cur[i];
        if (cur[i] != keyboard_register[i]) {
            cur[i] = keyboard_register[i];
            flag = true;
        }
    }
    
    return flag;
}

bool status(unsigned short* array, int key_code) {
    int row = key_code % 10;
    int col = key_code / 10 - 1;
    int bit = col + 8 * (row & 1);
    return array[row >> 1] & (1 << bit);
}

bool is_key_pressed(int key_code) {
    return status(cur, key_code);
}

bool is_key_held(int key_code) {
    return status(cur, key_code) && status(prev, key_code);
}

bool is_key_new_pressed(int key_code) {
    return status(cur, key_code) && !status(prev, key_code);
}
