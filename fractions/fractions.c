#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "fractions.h"

long long gcd(long long a, long long b) {
    a = llabs(a);
    b = llabs(b);

    if (b == 0) return (a == 0) ? 1 : a;
    return gcd(b, a % b);
}

double decValue(Fraction f) {
    return ((double) f.numerator) / f.denominator;
}

Fraction simplify(Fraction f) {
    Fraction simplified;
    long long div = gcd(f.numerator, f.denominator);
    simplified.numerator = f.numerator / div;
    simplified.denominator = f.denominator / div;
    
    if (simplified.denominator < 0) {
        simplified.numerator *= -1;
        simplified.denominator *= -1;
    }

    return simplified;
}

Fraction add(Fraction f1, Fraction f2) {
    Fraction res = {f1.numerator * f2.denominator + f2.numerator * f1.denominator, f1.denominator * f2.denominator};
    return simplify(res);
}

Fraction sub(Fraction f1, Fraction f2) {
    f2.numerator *= -1;
    return add(f1, f2);
}

Fraction mul(Fraction f1, Fraction f2) {
    Fraction res = {f1.numerator * f2.numerator, f1.denominator * f2.denominator};
    return simplify(res);
}

Fraction divide(Fraction f1, Fraction f2) {
    Fraction res = {f1.numerator * f2.denominator, f1.denominator * f2.numerator};
    return simplify(res);
}

int greater(Fraction f1, Fraction f2) {
    return f1.numerator * f2.denominator > f2.numerator * f1.denominator;
}

Fraction readFraction(int decimalDigits) {
    double number;
    Fraction res;

    scanf("%lf", &number);

    res.numerator = number * pow(10, decimalDigits);
    res.denominator = pow(10,decimalDigits);
    return res;
}