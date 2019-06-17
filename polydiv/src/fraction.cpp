#include "fraction.h"

#include <fxcg/keyboard.h>
#include <stdlib.h>

static int gcd(int a, int b) {
    while (a != b) {
        if (a == 1 || b == 1) {
            return 1;
        }

        if (a > b) {
            a -= b;
        } else {
            b -= a;
        }
    }

    return a;
}

static int lcm(int a, int b) {
    int big, small, inc;

    if (a > b) {
        big = a;
        small = b;
    } else {
        big = b;
        small = a;
    }

    inc = big;

    while (big % small != 0) {
        big += inc;
    }

    return big;
}

Fraction::Fraction(int num, int den) : num(num), den(den) {
    this->normalize();
    this->cancel();
}

Fraction::Fraction(const char* str) : num(0), den(0) {
    bool negative = str[0] == (char)KEY_CHAR_MINUS;
    str += negative;

    int* p = &num;
    for (int i = 0; str[i]; ++i) {
        if (str[i] == '/' || str[i] == (char)KEY_CHAR_DIV || str[i] == (char)KEY_CHAR_FRAC) {
            p = &den;
            continue;
        }

        *p *= 10;
        *p += str[i] - '0';
    }

    if (negative) {
        this->num = -this->num;
    }

    if (!den) {
        den = 1;
    } else {
        this->cancel();
    }
}

Fraction& Fraction::cancel() {
    int div = gcd(abs(num), abs(den));
    num /= div;
    den /= div;

    return *this;
}

Fraction& Fraction::operator+=(const Fraction& other) {
    int den_new = lcm(abs(den), abs(other.den));
    this->num = this->num * (den_new / this->den) + other.num * (den_new / other.den);
    this->den = den_new;

    return this->normalize();
}

Fraction& Fraction::operator-=(const Fraction& other) {
    Fraction negative = other;
    negative.num = -negative.num;
    return ((*this) += negative).normalize();
}

Fraction& Fraction::operator*=(const Fraction& other) {
    this->num *= other.num;
    this->den *= other.den;

    return this->normalize();
}

Fraction& Fraction::operator/=(const Fraction& other) {
    this->num *= other.den;
    this->den *= other.num;

    return this->normalize();
}

Fraction& Fraction::normalize() {
    if (den < 0) {
        num = -num;
        den = -den;
    }

    return *this;
}
