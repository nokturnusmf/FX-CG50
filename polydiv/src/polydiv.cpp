#include <fxcg/display.h>
#include <fxcg/keyboard.h>

#include "vec.h"
#include "fraction.h"
#include "util.h"

using Polynomial = Vector<Fraction>;

Polynomial divide(const Polynomial& num, const Polynomial& div) {
    Polynomial result(num.size() - div.size() + 1);

    Polynomial current(div.size());
    for (int i = 0; i < div.size(); ++i) {
        current.push_back(num[i]);
    }

    for (int i = 0; i < num.size() - div.size() + 1; ++i) {
        Fraction mul = (current[0] / div[0]).cancel();
        result.push_back(mul);

        for (int j = 1; j < div.size(); ++j) {
            current[j - 1] = current[j] - div[j] * mul;
        }

        current[div.size() - 1] = num[i + div.size()];
    }

    return result;
}

void print_fraction(int x, int y, const Fraction& frac, int mode, int color) {
    int len = PrintNumXY(x, y, frac.numerator(), mode, color);
    if (frac.denominator() != 1) {
        PrintXY(x + len, y, "  /", mode, color);
        PrintNumXY(x + len + 1, y, frac.denominator(), mode, color);
    }
}

void get_polynomial(Polynomial& poly, char c, int pos_x) {
    for (int i = 2;; ++i) {
        char buffer[32]; buffer[0] = c;
        memset(buffer + 1, 0, sizeof(buffer) - 1);

        while (!get_string(buffer, 32, pos_x, 1));

        if (buffer[0] == c) {
            return;
        } else {
            poly.push_back(buffer);
            print_fraction(pos_x, i, poly[poly.size() - 1], 0, TEXT_COLOR_PURPLE);
        }
    }
}

void clear() {
    char empty[25]; empty[sizeof(empty) - 1] = '\0';
    memset(empty, ' ', sizeof(empty) - 1);
    for (int i = 1; i < 8; ++i) {
        PrintXY(1, i, empty, 0, TEXT_COLOR_BLACK);
    }
}

int main() {
    int key;
    while (1) {
        clear();
        Polynomial num, div;
        get_polynomial(num, 'A', 1);
        get_polynomial(div, 'B', 11);
        clear();

        auto res = divide(num, div);
        PrintNumXY(1, 1, res.size(), 0, TEXT_COLOR_PURPLE);
        for (int i = 0; i < res.size(); ++i) {
            print_fraction(1, i + 2, res[i], 0, TEXT_COLOR_BLACK);
        }

        do {
            GetKey(&key);
        } while (key != KEY_CTRL_EXIT);
    }
}
