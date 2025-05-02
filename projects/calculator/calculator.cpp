#include "calculator.hpp"

BigInt::BigInt(int a) {
    if (a < 0) {
        digits.push_back(static_cast<uint32_t>(-a));
        this->neg();
    } else {
        digits.push_back(static_cast<uint32_t>(a));
    }
}

BigInt::BigInt(const BigInt &x) {
    this->digits = x.digits;
}

BigInt::BigInt(const std::vector<uint32_t> &vec) {
    digits = vec;
}

BigInt::BigInt(const std::string &s) {
    // if (s.substr(0, 2) == "0x" || s.substr(0,3) == "-0x") {
    //     fromHex(s);
    // } //ввод под формат 16-тиричной СС
    // fromHex(s);
    fromDecimal(s);
}

void
BigInt::neg() {
    uint32_t carry = 1;
    for (size_t i = 0; i < this->digits.size(); ++i) {
        this->digits[i] = ~this->digits[i] + carry;
        if (this->digits[i] != 0) {
            carry = 0;
        }
    }
    if (carry == 1) {
        digits.push_back(0xFFFFFFFF);
    }
}

void
BigInt::fromHex(const std::string &hexStr) {
    std::string hex = hexStr;
    bool isNegative = false;
    if (hex.substr(0, 1) == "-") {
        isNegative = true;
        hex = hex.substr(1);
    }

    std::reverse(hex.begin(), hex.end());
    for (size_t i = 0; i < hex.size(); i += 8) {
        std::string part = hex.substr(i, 8);
        std::reverse(part.begin(), part.end());
        digits.push_back(std::stoul(part, nullptr, 16));
    }
    if (isNegative) {
        this->neg();
        if ((digits.back() & 0x80000000) == 0) {
            digits.push_back(0xFFFFFFFF);
        }
    } else {
        if (!digits.empty() && (digits.back() & 0x80000000) != 0) {
            digits.push_back(0x00000000);
        }
    }
}

void
BigInt::fromDecimal(const std::string &str) {
    bool isNegative = str[0] == '-' ? true : false;
    std::string numStr = isNegative ? str.substr(1) : str;

    BigInt base(10);
    BigInt current(0);

    for (char c : numStr) {
        if (!std::isdigit(c)) {
            std::cerr << "invalid symbol : " << c << std::endl;
            // return;
        }
        int digitValue;
        current = current * base + BigInt(c - '0');
    }

    digits = current.digits;
    if (isNegative) {
        neg();
        if ((digits.back() & 0x80000000) == 0) {
            digits.push_back(0xFFFFFFFF);
        }
    } else {
        if (!digits.empty() && (digits.back() & 0x80000000) != 0) {
            digits.push_back(0x00000000);
        }
    }
}

void
BigInt::printBin() const {
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        std::cout << std::bitset<32>(*it) << ' ';
    }
    std::cout << std::endl;
}

BigInt
BigInt::add(const BigInt &x) const {
    std::vector<uint32_t> resultDigits;
    size_t maxSize = std::max(this->digits.size(), x.digits.size());
    uint64_t carry = 0;

    for (size_t i = 0; i < maxSize - 1; ++i) {
        uint64_t a = (i < this->digits.size()) ? this->digits[i] : 0;
        uint64_t b = (i < x.digits.size()) ? x.digits[i] : 0;
        uint64_t sum = a + b + carry;
        resultDigits.push_back(static_cast<uint32_t>(sum));
        carry = sum >> 32;
    } // сложение беззнаковых разрядов

    // сложение последнего знакового разряда
    uint64_t a = (maxSize - 1 < this->digits.size()) ? this->digits[maxSize - 1] : 0;
    uint64_t b = (maxSize - 1 < x.digits.size()) ? x.digits[maxSize - 1] : 0;
    uint64_t sum = a + b + carry;
    resultDigits.push_back(static_cast<uint32_t>(sum));

    bool signA = (this->digits.back() & 0x80000000) != 0;
    bool signB = (x.digits.back() & 0x80000000) != 0;
    bool resultSign = (resultDigits.back() & 0x80000000) != 0;

    // сравнение знаков (проверка переполнения)
    if ((signA == signB) && (signA != resultSign)) {
        // переполнение произошло, повторное сложение с увеличенным вектором
        resultDigits.push_back((signA) ? 0xFFFFFFFF : 0x00000000);
        carry = 0;
        for (size_t i = 0; i < maxSize; ++i) {
            a = (i < this->digits.size()) ? this->digits[i] : 0;
            b = (i < x.digits.size()) ? x.digits[i] : 0;
            sum = static_cast<uint64_t>(a) + b + carry;
            resultDigits[i] = static_cast<uint32_t>(sum);
            carry = sum >> 32;
        }
    }

    BigInt result;
    result.digits = resultDigits;
    return result;
}

BigInt
BigInt::sub(const BigInt &x) const {
    BigInt negated = x;
    negated.neg();
    return this->add(negated);
}

BigInt
BigInt::mul(const BigInt &x) const {
    if (*this == BigInt(0) || x == BigInt(0)) {
        return BigInt(0);
    }

    if (*this == BigInt(1)) {
        return x;
    }

    if (x == BigInt(1)) {
        return *this;
    }

    const BigInt &A = *this;
    const BigInt &B = x;

    bool resultNegative =
            ((A.digits.back() & 0x80000000) != 0) ^ ((B.digits.back() & 0x80000000) != 0);
    BigInt absA = (A.digits.back() & 0x80000000) != 0 ? -A : A;
    BigInt absB = (B.digits.back() & 0x80000000) != 0 ? -B : B;

    std::stack<BigInt> stackA, stackB;
    std::stack<size_t> stackPart;
    std::stack<int> stackStep;
    std::stack<BigInt> stackP0, stackP1, stackP2;

    stackA.push(absA);
    stackB.push(absB);
    stackPart.push(0);
    stackStep.push(0);
    stackP0.push(BigInt(0));
    stackP1.push(BigInt(0));
    stackP2.push(BigInt(0));

    BigInt result;

    while (!stackA.empty()) {
        BigInt A = stackA.top();
        BigInt B = stackB.top();
        size_t half = stackPart.top();
        int step = stackStep.top();
        BigInt p0 = stackP0.top();
        BigInt p1 = stackP1.top();
        BigInt p2 = stackP2.top();

        stackA.pop();
        stackB.pop();
        stackPart.pop();
        stackStep.pop();
        stackP0.pop();
        stackP1.pop();
        stackP2.pop();

        if (step == 0) {
            if (A.digits.size() <= 1 || B.digits.size() <= 1) {
                uint64_t res = 0;
                for (size_t i = 0; i < A.digits.size(); ++i) {
                    for (size_t j = 0; j < B.digits.size(); ++j) {
                        res += static_cast<uint64_t>(A.digits[i]) * B.digits[j];
                    }
                }
                result.digits[0] = static_cast<uint32_t>(res);
                result.digits.push_back(static_cast<uint32_t>(res >> 32));
                continue;
            }

            half = std::max(A.digits.size(), B.digits.size()) / 2;

            p0 = BigInt(0);
            p1 = BigInt(0);
            p2 = BigInt(0);

            BigInt ax(std::vector<uint32_t>(A.digits.begin() + half, A.digits.end()));
            BigInt b(std::vector<uint32_t>(A.digits.begin(), A.digits.begin() + half));
            BigInt cx(std::vector<uint32_t>(B.digits.begin() + half, B.digits.end()));
            BigInt d(std::vector<uint32_t>(B.digits.begin(), B.digits.begin() + half));

            stackA.push(b);
            stackB.push(d);
            stackPart.push(half);
            stackStep.push(1);
            stackP0.push(p0);
            stackP1.push(p1);
            stackP2.push(p2);

            stackA.push(ax);
            stackB.push(cx);
            stackPart.push(half);
            stackStep.push(2);
            stackP0.push(BigInt(0));
            stackP1.push(BigInt(0));
            stackP2.push(BigInt(0));

            stackA.push(b.add(ax));
            stackB.push(d.add(cx));
            stackPart.push(half);
            stackStep.push(3);
            stackP0.push(BigInt(0));
            stackP1.push(BigInt(0));
            stackP2.push(BigInt(0));
        } else if (step == 1) {
            p0 = result;
        } else if (step == 2) {
            p2 = result;
        } else if (step == 3) {
            p1 = result;
        } else if (step == 4) {
            BigInt p1Sub = p1.sub(p0).sub(p2);
            p2 = p2.shift_left(2 * half * 32);
            p1Sub = p1Sub.shift_left(half * 32);
            result = p2.add(p1Sub).add(p0);
        }

        if (step == 1 || step == 2 || step == 3) {
            stackA.push(A);
            stackB.push(B);
            stackPart.push(half);
            stackStep.push(4);
            stackP0.push(p0);
            stackP1.push(p1);
            stackP2.push(p2);
        }
    }

    if (resultNegative) {
        result.neg();
    }
    result.digits.erase(std::remove(result.digits.begin(), result.digits.end(), 0), result.digits.end());
    return result;
}

BigInt
BigInt::shift_left(size_t shift) const {
    BigInt result;
    size_t digit_shift = shift / 32;
    size_t bit_shift = shift % 32;
    uint32_t carry = 0;

    for (size_t i = 0; i < digits.size(); ++i) {
        uint64_t shifted = (static_cast<uint64_t>(digits[i]) << bit_shift) | carry;
        result.digits.push_back(static_cast<uint32_t>(shifted));
        carry = shifted >> 32;
    }

    if (carry) {
        result.digits.push_back(carry);
    }

    result.digits.insert(result.digits.begin(), digit_shift, 0);

    return result;
}

std::ostream &
operator<<(std::ostream &out, const BigInt &x) {
    if (x == BigInt(0)) {
        out << "0";
        return out;
    }

    bool isNegative = (x.digits.back() & 0x80000000) != 0;
    if (isNegative) {
        BigInt temp = x;
        temp.neg();
        out << "-0x";
        bool leadingZero = true;
        for (auto it = temp.digits.rbegin(); it != temp.digits.rend(); ++it) {
            if (leadingZero) {
                if (*it != 0) {
                    leadingZero = false;
                    out << std::hex << *it;
                }
            } else {
                out << std::hex << std::setw(8) << std::setfill('0') << *it;
            }
        }
    } else {
        out << "0x";
        bool leadingZero = true;
        for (auto it = x.digits.rbegin(); it != x.digits.rend(); ++it) {
            if (leadingZero) {
                if (*it != 0) {
                    leadingZero = false;
                    out << std::hex << *it;
                }
            } else {
                out << std::hex << std::setw(8) << std::setfill('0') << *it;
            }
        }
    }
    return out;
}

BigInt
BigInt::operator+() const {
    return *this;
}

BigInt
BigInt::operator-() const {
    BigInt result = *this;
    result.neg();
    return result;
}

BigInt
BigInt::operator+(const BigInt &b) const {
    return this->add(b);
}

BigInt
BigInt::operator-(const BigInt &a) const {
    return this->sub(a);
}

BigInt
BigInt::operator*(const BigInt &a) const {
    return this->mul(a);
}



bool
BigInt::operator>(const BigInt &x) const {
    bool signA = (this->digits.back() & 0x80000000) != 0; // 1 если отрицательное
    bool signB = (x.digits.back() & 0x80000000) != 0;

    if (signA && !signB) {
        return false; // A - отриц, B - положит
    }
    if (!signA && signB) {
        return true; // B - отриц, A - положит
    }

    // одинаковые знаки, разное число разрядов
    if (this->digits.size() != x.digits.size()) {
        if (signA) {
            return this->digits.size() < x.digits.size(); // для отриц
        } else {
            return this->digits.size() > x.digits.size();
        }
    }

    // одинаковые знаки и число разрядов
    for (int i = this->digits.size() - 1; i >= 0; --i) {
        if (this->digits[i] != x.digits[i]) {
            if (signA) {
                return this->digits[i] < x.digits[i]; // для отриц
            } else {
                return this->digits[i] > x.digits[i];
            }
        }
    }

    return false; // A и B равны
}

bool
BigInt::operator==(const BigInt &x) const {
    return this->digits == x.digits;
}

bool
BigInt::operator>=(const BigInt &x) const {
    return (*this > x) || (*this == x);
}

bool
BigInt::operator<(const BigInt &x) const {
    return !(*this >= x);
}

bool
BigInt::operator<=(const BigInt &x) const {
    return (*this < x) || (*this == x);
}

bool
BigInt::operator!=(const BigInt &x) const {
    return !(*this == x);
}



const double
Complex::real() {
    return re;
}

const double
Complex::imaginary() {
    return im;
}

Complex::Complex(double re, double im) {
    this->re = re;
    this->im = im;
}

Complex::Complex(const Complex &x) {
    this->re = x.re;
    this->im = x.im;
}

Complex::Complex(const std::string &s) {
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
    if (s[i] == '+' || s[i] == '-') { // знак
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

    if (s[start - 1] == '-') {
        im = -im;
    }

    if (i >= len || s[i] != 'i') {
        std::cerr << "incorrect input of an imaginary number" << std::endl;
        re = 0;
        im = 0;
    }
}

Complex
Complex::operator+(const Complex &x) const {
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
Complex::operator^(const int n) const { // по формуле Муавра
    double r = std::sqrt(this->re * this->re + this->im * this->im);
    double phi = std::atan2(this->im, this->re);
    double r_n = std::pow(r, n);
    double phi_n = n * phi;
    double re = r_n * std::cos(phi_n);
    double im = r_n * std::sin(phi_n);
    return Complex(re, im);
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

std::ostream &
operator<<(std::ostream &out, const Complex &x) {
    out << x.re;
    if (x.im != 0) {
        if (x.im > 0) {
            out << "+";
        }
        out << x.im << "i";
    }
    return out;
}

bool
isOperator(const char &c) {
    return ((c == '+') || (c == '-') || (c == '*') || 
            (c == '/') || (c == '(') || (c == ')') || (c == '^'));
}

bool
isValidChar(const char &c) { // все допустимые символы в калькуляторе
    return std::isdigit(c) || isOperator(c) || c == '.' || c == ' ' || c == 'i';
}

DataType
getExpressionType(const std::string &input) { // определение типа данных строки
    long long curNum = 0;

    for (char c : input) {
        if (c == 'i') {
            return COMPLEX;
        }
        if (std::isdigit(c)) {
            curNum = curNum * 10;
            curNum = curNum + (c - '0');
            continue;
        }
        if (isValidChar(c)) {
            if (curNum > std::numeric_limits<int>::max()) { // бигинт если любая
                                                            // последовательность длинее инта
                return BIG_INT;
            }
            curNum = 0;
            continue; // чтобы в инвалид не вывалиться
        }
        return INVALID;
    }
    if (curNum > int(std::numeric_limits<int>::max())) { // бигинт если любая
                                                         // последовательность длинее инта
        return BIG_INT;
    }
    return DOUBLE;
}


int
precedence(const std::string &op) {
    if (op == "+" || op == "-") {
        return 1;
    } else if (op == "*" || op == "/") {
        return 2;
    } else if (op == "^") {
        return 3;
    }
    return 0;
}

bool
isLeftAssociative(const std::string &op) {
    return op != "^";
}

std::vector<Token>
tokenizeBigInt(const std::string &input) {
    std::vector<Token> tokens;
    std::string currentToken;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '.') {
            throw CalculatorException("Decimal point in BigInt expression");
        }
        if (input[i] == '^') {
            throw CalculatorException("Can't use power with BigInt");
        }
        if (input[i] == '/') {
            throw CalculatorException("Can't use division with BigInt");
        }
        if (std::isdigit(input[i])) {
            if (currentToken.empty() && input[i] == '0') {
                throw CalculatorException("Integer can't start with zero(s)");
            }
            currentToken += input[i];
            continue;
        }
        if (!currentToken.empty()) { // если мы тут, то в цикле наткнулись на конец числа
            tokens.push_back(Token(NUMBER, currentToken));
            currentToken = "";
        }
        if (isOperator(input[i])) {
            tokens.push_back(Token(input[i]));
        }
    }
    if (!currentToken.empty()) {
        tokens.push_back(Token(NUMBER, currentToken));
    }
    if (tokens.back().type == OPERATOR) {
        throw CalculatorException("Can't use operator at the end of expression");
    }
    return tokens;
}

std::vector<Token>
tokenizeComplex(const std::string &input) { // токенизация строки с комплексными числами
    std::vector<Token> tokens;
    std::string currentToken;
    for (size_t i = 0; i < input.length(); ++i) {
        if ((i != input.length() - 1) && (input[i] == 'i') &&
            !(isOperator(input[i + 1]) || input[i + 1] == ' ')) {
            throw CalculatorException("Invalid character after imaginary unit");
        }
        if (input[i] == '.') {
            if (i > 0 && !std::isdigit(input[i - 1])) {
                throw CalculatorException("Invalid character before decimal dot");
            }
            if (i < input.length() - 1 && !std::isdigit(input[i + 1])) {
                throw CalculatorException("Invalid character after decimal dot");
            }
        }

        if (input[i] == 'i' || std::isdigit(input[i]) || input[i] == '.') {
            currentToken += input[i];
            continue;
        }
        if (!currentToken.empty()) { // если мы тут, то в цикле наткнулись на конец числа
            tokens.push_back(Token(NUMBER, currentToken));
            currentToken = "";
        }
        if (isOperator(input[i])) {
            tokens.push_back(Token(input[i]));
        }
    }
    if (!currentToken.empty()) {
        tokens.push_back(Token(NUMBER, currentToken));
    }
    if (tokens.back().type == OPERATOR) {
        throw CalculatorException("Can't use operator at the end of expression");
    }
    return tokens;
}

std::vector<Token>
tokenizeDouble(const std::string &input) {
    std::vector<Token> tokens;
    std::string currentToken;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '.') {
            if (i > 0 && !std::isdigit(input[i - 1])) {
                throw CalculatorException("Invalid character before decimal dot");
            }
            if (i < input.length() - 1 && !std::isdigit(input[i + 1])) {
                throw CalculatorException("Invalid character after decimal dot");
            }
        }
        if (std::isdigit(input[i]) || input[i] == '.') {
            currentToken += input[i];
            continue;
        }
        if (!currentToken.empty()) { // если мы тут, то в цикле наткнулись на конец числа
            tokens.push_back(Token(NUMBER, currentToken));
            currentToken = "";
        }
        if (isOperator(input[i])) {
            tokens.push_back(Token(input[i]));
        }
    }
    if (!currentToken.empty()) {
        tokens.push_back(Token(NUMBER, currentToken));
    }
    if (tokens.back().type == OPERATOR) {
        throw CalculatorException("Can't use operator at the end of expression");
    }
    return tokens;
}

std::queue<Token>
sortingStation(std::vector<Token> tokens) { // один алгоритм сортировочной станции на все типы строк
    std::queue<Token> output;
    std::stack<Token> operators;

    for (size_t i = 0; i < tokens.size(); ++i) {
        Token token = tokens[i];
        if (token.type == NUMBER) {
            output.push(token);
            continue;
        }
        if (token.type == OPERATOR) {
            while (!operators.empty()) {
                if ((precedence(operators.top().value) > precedence(token.value)) ||
                    ((precedence(operators.top().value) == precedence(token.value)) &&
                     isLeftAssociative(token.value))) {
                    output.push(operators.top());
                    operators.pop();
                } else {
                    break;
                }
            }
            operators.push(token);
        } else if (token.type == OPEN_PAREN) {
            operators.push(token);
        } else if (token.type == CLOSE_PAREN) {
            bool foundOpenParen = false;
            while (!operators.empty()) {
                Token top = operators.top();
                operators.pop();
                if (top.type == OPEN_PAREN) {
                    foundOpenParen = true;
                    break;
                }
                output.push(top);
            }
            if (!foundOpenParen) {
                throw CalculatorException("Invalid parentheses");
            }
        } else if (token.type == INVALID_NUM) {
            throw CalculatorException("Invalid token");
        }
    }

    while (!operators.empty()) {
        Token top = operators.top();
        operators.pop();
        if (top.type == OPEN_PAREN) {
            throw CalculatorException("Invalid parentheses");
        }
        output.push(top);
    }
    return output;
}

double
doubleCalculateRPN(std::queue<Token> &tokens) {
    std::stack<double> stack;
    while (!tokens.empty()) {
        Token token = tokens.front();
        tokens.pop();
        if (token.type == NUMBER) {
            stack.push(std::stod(token.value));
        } else if (token.type == OPERATOR) {
            if (stack.size() < 2) {
                throw CalculatorException("Invalid expression");
            }
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            switch (token.value[0]) {
            case '+':
                stack.push(a + b);
                break;
            case '-':
                stack.push(a - b);
                break;
            case '*':
                stack.push(a * b);
                break;
            case '/':
                stack.push(a / b);
                break;
            case '^':
                stack.push(std::pow(a, b));
                break;
            }
        }
    }
    if (stack.size() != 1) {
        throw CalculatorException("Invalid expression");
    }
    return stack.top();
}

Complex
complexCalculateRPN(std::queue<Token> &tokens) {
    std::stack<Complex> stack;
    while (!tokens.empty()) {
        Token token = tokens.front();
        tokens.pop();
        if (token.type == NUMBER) {
            if (token.value.back() == 'i') {
                stack.push(Complex(0.0, std::stod(token.value)));
            } else {
                stack.push(Complex(std::stod(token.value), 0.0));
            }
        } else if (token.type == OPERATOR) {
            if (stack.size() < 2) {
                throw CalculatorException("Invalid expression");
            }
            Complex b = stack.top();
            stack.pop();
            Complex a = stack.top();
            stack.pop();
            switch (token.value[0]) {
            case '+':
                stack.push(a + b);
                break;
            case '-':
                stack.push(a - b);
                break;
            case '*':
                stack.push(a * b);
                break;
            case '/':
                stack.push(a / b);
                break;
            case '^':
                stack.push(a ^ int(b.real()));
                break;
            }
        }
    }
    if (stack.size() != 1) {
        throw CalculatorException("Invalid expression");
    }
    return stack.top();
}

BigInt
bigIntCalculateRPN(std::queue<Token> &tokens) {
    std::stack<BigInt> stack;
    while (!tokens.empty()) {
        Token token = tokens.front();
        tokens.pop();
        if (token.type == NUMBER) {
            stack.push(BigInt(token.value));
        } else if (token.type == OPERATOR) {
            if (stack.size() < 2) {
                throw CalculatorException("Invalid expression");
            }
            BigInt b = stack.top();
            stack.pop();
            BigInt a = stack.top();
            stack.pop();
            switch (token.value[0]) {
            case '+':
                stack.push(a + b);
                break;
            case '-':
                stack.push(a - b);
                break;
            case '*':
                stack.push(a * b);
                break;
            }
        }
    }
    if (stack.size() != 1) {
        throw CalculatorException("Invalid expression");
    }
    return stack.top();
}

bool
isExpressionCharactersValid(const std::string &input) {
    for (char c : input) {
        if (!isValidChar(c)) {
            return false;
        }
    }
    return true;
}

void
process_input(const std::string &input) {
    DataType type = getExpressionType(input); // возвращает тип строки
    if (!isExpressionCharactersValid(input)) {
        throw CalculatorException("Found invalid character");
    }
    std::vector<Token> tokens;
    std::queue<Token> sortedTokens;
    switch (type) {
    case COMPLEX:
        std::cout << "COMPLEX" << std::endl;
        tokens = tokenizeComplex(input);       // токенизация строки
        sortedTokens = sortingStation(tokens); // сортировочная станция
        {
            Complex answer =
                    complexCalculateRPN(sortedTokens); // вычисление в обратной польской нотации
            std::cout << "result: " << answer << std::endl;
        }

        break;
    case BIG_INT:
        std::cout << "BIG_INT " << std::endl;
        tokens = tokenizeBigInt(input);        // токенизация строки
        sortedTokens = sortingStation(tokens); // сортировочная станция
        {
            BigInt answer =
                    bigIntCalculateRPN(sortedTokens); // вычисление в обратной польской нотации
            std::cout << "result: " << answer << std::endl;
        }
        break;
    case DOUBLE:
        std::cout << "DOUBLE" << std::endl;
        tokens = tokenizeDouble(input);        // токенизация строки
        sortedTokens = sortingStation(tokens); // сортировочная станция
        {
            double answer =
                    doubleCalculateRPN(sortedTokens); // вычисление в обратной польской нотации
            std::cout << "result: " << answer << std::endl;
        }
        break;
    case INVALID:
    default:
        throw CalculatorException("Invalid input");
        break;
    }
}

int
main(void) {
    std::string input;
    std::cout << "Enter an expression: ";
    std::getline(std::cin, input);
    try {
        process_input(input);
    } catch (std::exception &e) {
        std::cout << "\033[1;31mException occured while calculating expression: \033[0m"
                  << e.what();
    }
    return 0;
}