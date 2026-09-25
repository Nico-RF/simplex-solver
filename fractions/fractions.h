#ifndef FRACTIONS_H
#define FRACTIONS_H
typedef struct Fraction {
    long long numerator, denominator;
} Fraction;

double decValue(Fraction f);
Fraction simplify(Fraction f);
Fraction add(Fraction f1, Fraction f2);
Fraction sub(Fraction f1, Fraction f2);
Fraction mul(Fraction f1, Fraction f2);
Fraction divide(Fraction f1, Fraction f2);
int greater(Fraction f1, Fraction f2);
Fraction readFraction(int decimalDigits);
#endif