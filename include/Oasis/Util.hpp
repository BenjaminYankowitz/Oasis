#ifndef OASIS_UTIL_HPP
#define OASIS_UTIL_HPP
#include "Oasis/Add.hpp"
#include "Oasis/Imaginary.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Real.hpp"
namespace Oasis::Util {
class IntegerComplex {
public:
    IntegerComplex(long long R);
    IntegerComplex(long long R, long long I);
    long long getReal() const;
    long long getImaginary() const;
    std::unique_ptr<Expression> getExpression() const;
    long long absSquared() const;
    IntegerComplex conj() const;
    bool operator==(IntegerComplex rhs) const;
    IntegerComplex& operator+=(const IntegerComplex& rhs);
    IntegerComplex& operator*=(const IntegerComplex& rhs);
    IntegerComplex& operator-=(const IntegerComplex& rhs);
    IntegerComplex& operator/=(const IntegerComplex& rhs);
    IntegerComplex& operator%=(const IntegerComplex& rhs);
    bool operator<(const IntegerComplex& rhs) const;

private:
    long long real;
    long long imaginary;
};
IntegerComplex operator+(IntegerComplex lhs, const IntegerComplex& rhs);
IntegerComplex operator-(IntegerComplex lhs, const IntegerComplex& rhs);
IntegerComplex operator*(IntegerComplex lhs, const IntegerComplex& rhs);
IntegerComplex operator/(IntegerComplex lhs, const IntegerComplex& rhs);
IntegerComplex operator%(IntegerComplex lhs, const IntegerComplex& rhs);

class Complex {
public:
    Complex(double R);
    Complex(double R, double I);
    double getReal() const;
    double getImaginary() const;
    std::unique_ptr<Expression> getExpression() const;
    double absSquared() const;
    Complex conj() const;
    bool operator==(Complex rhs) const;
    Complex& operator+=(const Complex& rhs);
    Complex& operator*=(const Complex& rhs);
    Complex& operator-=(const Complex& rhs);
    Complex& operator/=(const Complex& rhs);
    bool operator<(const Complex& rhs) const;

private:
    double real;
    double imaginary;
};

Complex operator+(Complex lhs, const Complex& rhs);
Complex operator-(Complex lhs, const Complex& rhs);
Complex operator*(Complex lhs, const Complex& rhs);
Complex operator/(Complex lhs, const Complex& rhs);

class PrimesList {
public:
    static long long get(size_t n) { return primes[n]; }
    static void generatePrimesUpToN(long long num);

private:
    static std::vector<long long> primes;
};

Oasis::Add<Oasis::Real, Oasis::Multiply<Oasis::Real, Oasis::Imaginary>> pairToComp(double a, double b);

long long gcf(long long a, long long b);
bool isInt(double n);
Oasis::Real abs(const Oasis::Expression& exp);
std::vector<std::unique_ptr<Expression>> getAllRationalPolynomialRoots(std::vector<IntegerComplex>& C);
} // Oasis::Util

#endif // OASIS_UTIL_HPP