#ifndef UTIL_H
#define UTIL_H

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}

int utoa(unsigned long val, char* str, int base = 10);
int itoa(long val, char* str, int base = 10);

int PrintNumXY(int x, int y, long num, int mode, int color);

#endif // UTIL_H
