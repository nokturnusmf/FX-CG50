#ifndef UTIL_H
#define UTIL_H

int utoa(unsigned long val, char* str, int base = 10);
int itoa(long val, char* str, int base = 10);

int PrintNumXY(int x, int y, long num, int mode, int color);

bool get_string(char* buffer, int length, int pos_x, int pos_y);

#endif // UTIL_H
