#ifndef CALC_H
#define CALC_H

#include "lexer.h"
#include <queue>
#include <stack>
#include <math.h>

using std::queue;
using std::stack;

class Calc {
private:
    stack<float> num_stack;
    stack<Token> op_stack;
public:
    Calc(){}
    ~Calc(){}
    // After the parser converts the lex list into Reverse-Polish Notation, it can be evaluated with this function.
    float evalRPN(queue<LexItem> output_queue);
};

#endif // CALC_H