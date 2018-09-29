#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include "vec.h"
#include "fraction.h"
#include "util.h"

using Polynomial = Vector<Fraction>;

bool get_string(char* buffer, int length, int pos_x, int pos_y) {
    int start = 0;
    int cursor = 0;

    DisplayMBString((unsigned char*)buffer, start, cursor, pos_x, pos_y);

    int key;
    while (1) {
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
}

Polynomial divide(const Polynomial& num, const Polynomial& div) {
    Polynomial result(num.size() - div.size() + 1);
    
    Polynomial current(div.size());
    for (int i = 0; i < div.size(); ++i) {
        current.push_back(num[i]);
    }
    
    for (int i = 0; i < num.size() - div.size() + 1; ++i) {
        Fraction mul = current[0] / div[0];
        result.push_back(mul);
        
        for (int j = 1; j < div.size(); ++j) {
            current[j - 1] = current[j] - div[j] * mul;
        }
        
        current[div.size() - 1] = num[i + div.size()];
    }
    
    return result;
}

int main() {
    int key;
    while (1) {
        Polynomial num, div;        
        
        int n = 1;
        while (1) {
            char buffer[32] = "A";
            while (!get_string(buffer, 32, 1, 1));
            if (buffer[0] == 'A') {
                break;
            } else {
                num.push_back(buffer);
                char temp[16] = "               ";
                itoa(num[num.size() - 1].numerator(), temp + 2);
                PrintXY(1, ++n, temp, 0, TEXT_COLOR_PURPLE);
            }
        }
        
        n = 1;
        while (1) {
            char buffer[32] = "B";
            while (!get_string(buffer, 32, 1, 1));
            if (buffer[0] == 'B') {
                break;
            } else {
                div.push_back(buffer);
                char temp[16] = "               ";
                itoa(div[div.size() - 1].numerator(), temp + 2);
                PrintXY(5, ++n, temp, 0, TEXT_COLOR_PURPLE);
            }
        }
        
        auto res = divide(num, div);
        char c[16] = "               ";
        itoa(res.size(), c + 2);
        PrintXY(9, 1, c, 0, TEXT_COLOR_PURPLE);
        
        for (int i = 0; i < res.size(); ++i) {
            char str[16] = "               ";
            itoa(res[i].cancel().numerator(), str + 2);
            PrintXY(9, i + 2, str, 0, TEXT_COLOR_BLACK);
            itoa(res[i].cancel().numerator(), str + 2);
            PrintXY(13, i + 2, str, 0, TEXT_COLOR_BLACK);
        }
        
        do {
            GetKey(&key);
        } while (key != KEY_CTRL_EXIT);
    }
}

