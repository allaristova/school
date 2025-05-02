#ifndef CALC_H
#define CALC_H

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>


enum DataType { // возможные типы данных строки или несоответствующий ни одному из них
    COMPLEX,
    BIG_INT,
    DOUBLE,
    INVALID
};

enum TokenType {
    NUMBER, // type 0
    OPERATOR,
    OPEN_PAREN,
    CLOSE_PAREN,
    INVALID_NUM
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string &v) {
        type = t;
        value = v;
    }

    Token(const char &c) { // конструктор оператора и скобок из знака
        switch (c) {
        case '(':
            type = OPEN_PAREN;
            break;

        case ')':
            type = CLOSE_PAREN;
            break;

        case '+':
        case '-':
        case '/':
        case '^':
        case '*':
            type = OPERATOR;
            break;

        default:
            type = INVALID_NUM;
            break;
        }
        value = std::string(1, c);
    }
};

class CalculatorException : public std::exception {
    std::string _msg;

public:
    CalculatorException(const std::string &msg)
        : _msg(msg) {}

    const char *what() const noexcept override {
        return _msg.c_str();
    }
};

class BigInt {
private:
    std::vector<uint32_t> digits;

public:
    BigInt(int a = 0);
    BigInt(const std::string &s);
    BigInt(const std::vector<uint32_t> &vec);
    BigInt(const BigInt &x);
    void neg();
    void printBin() const;
    void fromHex(const std::string &hexStr);

    BigInt add(const BigInt &x) const;
    BigInt sub(const BigInt &x) const;
    BigInt mul(const BigInt &x) const;
    BigInt shift_left(size_t shift) const;

    BigInt operator+(const BigInt &b) const;
    BigInt operator-(const BigInt &a) const;
    BigInt operator*(const BigInt &a) const;

    BigInt operator+() const;
    BigInt operator-() const;

    bool operator>(const BigInt &x) const;
    bool operator<(const BigInt &x) const;
    bool operator>=(const BigInt &x) const;
    bool operator<=(const BigInt &x) const;
    bool operator==(const BigInt &x) const;
    bool operator!=(const BigInt &x) const;
    void fromDecimal(const std::string &str);
    friend std::ostream &operator<<(std::ostream &out, const BigInt &x);
};


class Complex {
private:
    double re;
    double im;

public:
    Complex(double re = 0, double im = 0);
    Complex(const Complex &x);
    Complex(const std::string &s);

    Complex operator+(const Complex &x) const;
    Complex operator+(const int x) const;
    Complex operator-(const Complex &x) const;
    Complex operator-(const int x) const;
    Complex operator*(const Complex &x) const;
    Complex operator*(const int x) const;

    Complex operator/(const Complex &x) const;
    Complex operator/(const int x) const;
    Complex operator^(const int x) const;

    Complex operator+() const;
    Complex operator-() const;

    bool operator>(const Complex &x) const;
    bool operator<(const Complex &x) const;
    bool operator>=(const Complex &x) const;
    bool operator<=(const Complex &x) const;
    bool operator==(const Complex &x) const;
    bool operator!=(const Complex &x) const;

    const double real();
    const double imaginary();
    friend std::ostream &operator<<(std::ostream &out, const Complex &x);
};


#endif
