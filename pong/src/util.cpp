#include "util.h"

#include <fxcg/display.h>

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

