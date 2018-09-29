#ifndef UTIL_H
#define UTIL_H

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}

void itoa(long num, char* str, int base = 10);

void PrintNumXY(int x, int y, int i, int mode, int color);

void rng_init(unsigned int seed);
unsigned int rng_next();

#endif // UTIL_H
