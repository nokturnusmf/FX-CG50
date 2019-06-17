#include "util.h"

#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include <string.h>

int utoa(unsigned long val, char* str, int base) {
    const char* digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (val == 0) {
        str[0] = '0';
        str[1] = '\0';
        return 1;
    } else {
        str[0] = '\0';

        int i = 0;
        while (val) {
            memmove(str + 1, str, ++i);
            str[0] = digits[val % base];
            val /= base;
        }

        return i;
    }
}

int itoa(long val, char* str, int base) {
    if (val < 0) {
        str[0] = '-';
        return 1 + utoa(-val, str + 1, base);
    } else {
        return utoa(val, str, base);
    }
}

int PrintNumXY(int x, int y, long num, int mode, int color) {
    char buffer[16] = { ' ', ' ' };
    int length = itoa(num, buffer + 2);
    PrintXY(x, y, buffer, mode, color);
    return length;
}

bool get_string(char* buffer, int length, int pos_x, int pos_y) {
    int start = 0;
    int cursor = 0;
    while (buffer[++cursor] && cursor < length);

    DisplayMBString((unsigned char*)buffer, start, cursor, pos_x, pos_y);

    int key;
    while (cursor < length) {
        GetKey(&key);

        if (key == KEY_CTRL_EXE) {
            return true;
        } else if (key == KEY_CTRL_EXIT) {
            return false;
        }

        if (key && key < 30000) {
            cursor = EditMBStringChar((unsigned char*)buffer, 32, cursor, key);
            DisplayMBString((unsigned char*)buffer, start, cursor, pos_x, pos_y);
        } else {
            EditMBStringCtrl((unsigned char*)buffer, 32, &start, &cursor, &key, pos_x, pos_y);
        }
    }

    return true;
}
