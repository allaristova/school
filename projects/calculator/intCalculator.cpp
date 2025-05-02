#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

std::vector<std::string> 
toVector(std::string expression, bool& error) {
    std::vector<std::string> tokens;
    std::string token;
    
    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (isspace(c)) {
            if (!tokens.empty() && tokens.back() == "*/") {
                tokens.back() = "*";
            }
            if (!token.empty()) {
                if (token == "-") {
                    std::cerr << "\x1B[31m\x1B[31mError:\033[0m\033[0m \"-\" is written without a space for signed numbers" << c << std::endl;
                    error = true;
                    return tokens;
                }
                tokens.push_back('n' + token);
                token.clear();
            }
            continue;
        } else if (isdigit(c)) {
            if (!tokens.empty() && tokens.back() == "*/") {
                tokens.back() = "*";
            }
            token += c;
        } else if (c == '-') {
            if (!tokens.empty() && tokens.back() == "*/") {
                tokens.back() = "*";
            }
            if (!token.empty()) {
                tokens.push_back('n' + token);
                token.clear();
            }
            if (tokens.empty()) {
                token += c;
            } else if (tokens.back()[0] == 'n' || tokens.back() == ")") {
                tokens.push_back(std::string(1, c));
            } else {
                token += c;
            }   
        } else if (c == '*') {
            if (!token.empty()) {
                tokens.push_back('n' + token);
                token.clear();
            }
            if (tokens.empty()) {
                std::cerr << "\x1B[31mError:\033[0m a mathematical expression cannot begin with " << c << std::endl;
                error = true;
                return tokens;
            }
            if (tokens.back() == "*/") {
                tokens.back() = "**";
            } else {
                tokens.push_back("*/");
            }
        } else if (c == '+' || c == '%' || c =='/' || c =='(' || c ==')') {
            if (!tokens.empty() && tokens.back() == "*/") {
                tokens.back() = "*";
            }
            if (!token.empty()) {
                tokens.push_back('n' + token);
                token.clear();
            }
            if (tokens.empty() && c != '(' && c != ')') {
                std::cerr << "\x1B[31mError:\033[0m a mathematical expression cannot begin with " << c << std::endl;
                error = true;
                return tokens;
            }
            tokens.push_back(std::string(1, c));
        } else {
            std::cerr << "\x1B[31mError:\033[0m expression contains invalid characters" << std::endl;
            error = true;
            return tokens;
        }
    }
    if (!tokens.empty() && tokens.back() == "*/") {
        tokens.back() = "*";
    }
    if (!token.empty()) {
        tokens.push_back('n' + token);
    }
    return tokens;
}

std::vector<std::string> 
toPostfixNotation(std::vector<std::string> tokens, bool& error) {
    std::stack<std::string> operators;
    std::vector<std::string> postfixTokens;
    for(int i = 0; i < tokens.size(); ++i){
        if (tokens[i][0] == 'n') {
            postfixTokens.push_back(tokens[i]);
        } else if (tokens[i] == "**") {
            while (!operators.empty() && operators.top() == "**") {
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
            operators.push(tokens[i]);
        } else if (tokens[i] == "*" || tokens[i] == "/" || \
                tokens[i] == "%") {
            while (!operators.empty() && (operators.top() == "*" || operators.top() == "/" || \
                    operators.top() == "%" || operators.top() == "**")) {
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
            operators.push(tokens[i]);
        } else if (tokens[i] == "+" || tokens[i] == "-") {
            while (!operators.empty() && operators.top()!= "(") {
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
            operators.push(tokens[i]);
        } else if (tokens[i] == "(") {
            operators.push(tokens[i]);
        } else if (tokens[i] == ")") {
            while (!operators.empty() && operators.top()!= "(") {
                postfixTokens.push_back(operators.top());
                operators.pop();
            }
            if (operators.top() != "(") {
                std::cerr << "\x1B[31mError:\033[0m extra character: " << tokens[i] << std::endl;
                error = true;
                return postfixTokens;
            }
            operators.pop();
        }
    }
    while (!operators.empty()) {
        if (operators.top() == "(") {
            std::cerr << "\x1B[31mError:\033[0m extra character: " << operators.top() << std::endl;
            error = true;
            return postfixTokens;
        }
        postfixTokens.push_back(operators.top());
        operators.pop();
    }
    return postfixTokens;
}

int
calculate(std::vector<std::string> tokens, bool& error) {
    std::stack<int> operands;
    int operand1;
    int operand2;
    for(int i = 0; i < tokens.size(); ++i) {
        if (tokens[i][0] == 'n') {
            if (tokens[i] == "n-") {
                std::cerr << "\x1B[31mError:\033[0m insufficient operands for the operation: " << tokens[i][1] << std::endl;
                error = true;
                return -1;
            }
            tokens[i][0] = ' ';
            operands.push(std::stoi(tokens[i]));
        } else if (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" \
                || tokens[i] == "/" || tokens[i] == "**" || tokens[i] == "%") {
            if (operands.size() < 2) {
                std::cerr << "\x1B[31mError:\033[0m insufficient operands for the operation: " << tokens[i] << std::endl;
                error = true;
                return -1;
            }
            operand2 = operands.top(); 
            operands.pop();
            operand1 = operands.top(); 
            operands.pop();
            if (tokens[i] == "+") {
                operands.push(operand1 + operand2);
            } else if (tokens[i] == "-") {
                operands.push(operand1 - operand2);
            } else if (tokens[i] == "*") {
                operands.push(operand1 * operand2);
            } else if (tokens[i] == "/") {
                if (operand2 == 0) {
                    std::cerr << "\x1B[31mError:\033[0m division by zero" << std::endl;
                    error = true;
                    return -1;
                }
                operands.push(operand1 / operand2);
            } else if (tokens[i] == "**") {
                operands.push(pow(operand1, operand2));
            } else if (tokens[i] == "%") {
                if (operand2 == 0) {
                    std::cerr << "\x1B[31mError:\033[0m division by zero" << std::endl;
                    error = true;
                    return -1;
                }
                operands.push(operand1 % operand2);
            }
        }
    }
    if (operands.size() != 1) {
        std::cerr << "\x1B[31mError:\033[0m too many operands" << std::endl;
        error = true;
        return -1;
    }
    return operands.top();
}


int
main(void) {
    std::string expression;
    std::vector<std::string> tokens;
    bool error;
    std::cout << "Hello!" << std::endl;
    while (true) {
        error = false;
        std::cout << "Enter a mathematical expression:" << std::endl;
        std::getline(std::cin, expression);
        tokens = toVector(expression, error);
        if (!error) { 
            std::vector<std::string> postfixTokens = toPostfixNotation(tokens, error);
            if (!error) {
                int result = calculate(postfixTokens,  error);
                if (!error) {
                    std::cout << "Result: \x1B[36m" << result << "\033[0m" << std::endl;
                }
            }
        }
        std::cout << "\n" << std::endl;
    }
    return 0;
}