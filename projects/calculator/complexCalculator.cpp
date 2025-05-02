#include <iostream>
#include "complexCalculator.hpp"


Complex::Complex(double re, double im) {
    this->re = re;
    this->im = im;
}

Complex::Complex(const Complex &x) {
    this->re = x.re;
    this->im = x.im;
}

Complex::Complex(const string &s) {
    size_t len = s.length();
    size_t i = 0;
    bool isNeg = 0;
    while (i < len && std::isspace(s[i])) {
        i++;
    }

    size_t start = i;
    if (s[i] == '-' || s[i] == '+') {
        i++;
    }
    while (i < len && (std::isdigit(s[i]) || s[i] == '.')) { // действительная часть 
        i++;
    }
    re = std::stod(s.substr(start, i - start));


    while (i < len && std::isspace(s[i])) {
        i++;
    }

    start = i;
    if (s[i] == '+' || s[i] == '-') { //знак
        i++;
    }

    while (i < len && std::isspace(s[i])) {
        i++;
    }


    start = i;
    while (i < len && (std::isdigit(s[i]) || s[i] == '.')) { // мнимая часть 
        i++;
    }
    im = std::stod(s.substr(start, i - start));


    while (i < len && std::isspace(s[i])) {
        i++;
    }

    if (i >= len || s[i] != 'i') {
        std::cerr << "incorrect input of an imaginary number" << std::endl;
        re = 0;
        im = 0;
    }

}


Complex
Complex::operator+(const Complex &x) const{
    return Complex(re + x.re, im + x.im);
}

Complex
Complex::operator+(const int x) const {
    return Complex(re + x, im);
}

Complex
Complex::operator-(const Complex &x) const {
    return Complex(re - x.re, im - x.im);
}

Complex
Complex::operator-(const int x) const {
    return Complex(re - x, im);
}

Complex
Complex::operator*(const Complex &x) const {
    return Complex(re * x.re - im * x.im, re * x.im + im * x.re);
}

Complex
Complex::operator*(const int x) const {
    return Complex(re * x, im * x);
}

Complex
Complex::operator/(const Complex &x) const {
    double den = x.re * x.re + x.im * x.im;
    return Complex((re * x.re + im * x.im) / den, (x.re * im - re * x.im) / den);
}

Complex
Complex::operator/(const int x) const {
    return Complex(re / x, im / x);
}


Complex
Complex::operator^(const int x) const {
    Complex res(1, 0);
    for(int i = 0; i < x; ++i) {
        res = res * (*this);
    }
    return res;
}


bool 
Complex::operator>(const Complex &x) const {
    return (re * re + im * im) > (x.re * x.re + x.im * x.im);
}

bool 
Complex::operator<(const Complex &x) const {
    return (re * re + im * im) < (x.re * x.re + x.im * x.im);
}

bool 
Complex::operator>=(const Complex &x) const {
    return !(*this < x);
}

bool 
Complex::operator<=(const Complex &x) const {
    return !(*this > x);
}


bool 
Complex::operator==(const Complex &x) const {
    return (re == x.re) && (im == x.im);
}

bool 
Complex::operator!=(const Complex &x) const {
    return !(*this == x);
}


std::ostream 
&operator<<(std::ostream &out, const Complex &x) {
    out << x.re;
    if (x.im != 0) {
        if (x.im > 0) {
            out << "+";
        }
        out << x.im << "i";
    }
    return out;
}


// test
int
main(void) {
    Complex a("-1 + 2i");
    Complex b("3  -4i");

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / b = " << a / b << std::endl;

    std::cout << "a > b " << (a > b) << std::endl;
    std::cout << "a < b " << (a < b) << std::endl;
    std::cout << "a = b " << (a == b) << std::endl;

    std::cout << "a ^ 3 " << (a * a * a) << " " << (a ^ 3) << std::endl;


    return 0;
}