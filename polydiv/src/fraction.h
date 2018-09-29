#ifndef FRACTION_H
#define FRACTION_H

class Fraction {
public:
    Fraction() : Fraction(0, 1) {}
    Fraction(int num) : Fraction(num, 1) {}
    Fraction(int num, int den);
    
    Fraction(const char* str, int length = 0);
    
    Fraction(const Fraction& other) { *this = other; }
    Fraction(Fraction&& other) { *this = other; }

    ~Fraction() = default;
    
    Fraction& operator=(const Fraction& other) = default;
    Fraction& operator=(Fraction&& other) = default;
    
    int numerator() const { return num; }
    int denominator() const { return den; }
    
    Fraction& cancel();
    
    Fraction operator+(const Fraction& other) const { return Fraction(*this) += other; }
    Fraction operator-(const Fraction& other) const { return Fraction(*this) -= other; }
    Fraction operator*(const Fraction& other) const { return Fraction(*this) *= other; }
    Fraction operator/(const Fraction& other) const { return Fraction(*this) /= other; }
    
    Fraction& operator+=(const Fraction& other);
    Fraction& operator-=(const Fraction& other);
    Fraction& operator*=(const Fraction& other);
    Fraction& operator/=(const Fraction& other);
    
private:
    Fraction& normalize();
    
    int num;
    int den;
};

#endif // FRACTION_H

