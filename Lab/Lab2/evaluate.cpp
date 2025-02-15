// Project identifier: 1CAEF3A0FEDD0DEC26BA9808C69D4D22A9962768

#include "evaluate.hpp"
#include <iostream>
#include <cstdint>
#include <stack>
using namespace std;

std::int64_t evaluate(std::string const& expression) {
    // Your implementation goes here
    stack<int64_t> numbers;
    stack<char> operand;
    int64_t num1 = 0;
    int64_t num2 = 0;
    char op = '\0';
    for (size_t i = 0; i < expression.size(); i++){
        if (expression[i] < '0'){
            operand.push(expression[i]);
        }
        else{
            numbers.push(expression[i] - '0');
        }
        if (!operand.empty() && (numbers.size() > 1)){
            num2 = numbers.top();
            numbers.pop();
            op = operand.top();
            operand.pop();
            num1 = numbers.top();
            numbers.pop();
            numbers.push(calculate(num1, num2, op));
        }
    }
    num1 = numbers.top();
    numbers.pop();
    return num1;
}

int64_t calculate(int64_t const& num1, int64_t const& num2, char const& operand){
    switch (operand)
    {
    case '+':
        return (num1 + num2);
    case '-':
        return (num1 - num2);
    case '*':
        return (num1 * num2);
    case '/':
        return (num1 / num2);
    default:
        cout << "Invalid operand." << endl;
        exit(1);
        break;
    }
}
