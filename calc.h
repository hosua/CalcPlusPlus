#ifndef CALC_H
#define CALC_H

#include "lexer.h"
#include <queue>
#include <stack>
#include <math.h>

using std::queue;
using std::stack;

// Any errors will arbritrarily set the value to the min of a double
#define ERR_RES std::numeric_limits<double>::min()

// Used for checking if a number is close enough to zero to be considered zero.
#define EPSILON std::numeric_limits<double>::epsilon() * 10

// Set of all trig functions
extern set<Token> trig_set;

class Calc {
public:
    Calc(){}
    ~Calc(){}
    // After the parser converts the lex list into Reverse-Polish Notation, it can be evaluated with this function.
    double evalRPN(queue<LexItem> output_queue);

private:
    double convertToRadian(float num){ return num * (M_PI/180); }

    stack<double> num_stack;
    stack<Token> op_stack;
};

#endif // CALC_H