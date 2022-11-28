#include "calc.h"

#include <math.h>

#include <limits>

// Set of all normal trig functions
set<Token> trig_set = {SIN, COS, TAN, CSC, SEC, COT, ASIN, ACOS, ATAN};

long double Calc::evalRPN(queue<LexItem> output_queue) {
    while (!output_queue.empty()) {
        LexItem lex = output_queue.front();
        Token tok = lex.getToken();
        output_queue.pop();
        if (tok == NUM) {
            num_stack.push(lex.getVal());
            // If token is a constant
        } else if (const_set.find(tok) != const_set.end()) {
            switch (tok) {
                case PI:
                    num_stack.push(M_PI);
                    break;
                case E:
                    num_stack.push(EULERS_NUM);
                    break;
                case ANS:
                    num_stack.push(prev_ans);
                    break;
                default:
                    cerr << "Error: const error in calc\n";
                    return ERR_RES;
            }
            // If token is an operator
        } else if (op_set.find(tok) != op_set.end()) {
            // If the num_stack is empty for any of these calls, there must be a
            // syntax error somewhere.
            if (num_stack.empty()) {
                cerr << "ERROR: Detected operator with no numbers\n";
                return ERR_RES;
            }
            long double b = num_stack.top();
            num_stack.pop();
            if (num_stack.empty()) {
                cerr << "ERROR: Detected consecutive operators\n";
                return ERR_RES;
            }
            long double a = num_stack.top();
            num_stack.pop();
            switch (tok) {
                case PLUS:
                    num_stack.push(a + b);
                    break;
                case MIN:
                    num_stack.push(a - b);
                    break;
                case MULT:
                    num_stack.push(a * b);
                    break;
                case DIV:
                    // Divide by zero error
                    if (abs(b) <= EPSILON) {
                        cerr << "DIVIDE BY ZERO ERROR\n";
                        return ERR_RES;
                    }
                    num_stack.push(a / b);
                    break;
                case EXP:
                    num_stack.push(powl(a, b));
                    break;
                case MOD:
                    num_stack.push(fmodl(a, b));
                    break;

                default:
                    cerr << "ERROR: Calc error in ops\n";
                    return ERR_RES;
            }
            // If a trig function
        } else if (trig_set.find(tok) != trig_set.end()) {
            long double input = (!radians_mode)
                                    ? convertToRadians(num_stack.top())
                                    : num_stack.top();
            num_stack.pop();
            switch (tok) {
                case SIN:
                    input = sinl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(sin(input));
                    break;
                case COS:
                    // This needs to be done to avoid floating-point rounding
                    // errors
                    input = cosl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;
                case TAN:
                    if (fmodl(abs(input), M_PI_2) == 0) {
                        cerr << "DOMAIN ERROR for TAN\n";
                        return ERR_RES;
                    }
                    num_stack.push(tanl(input));
                    break;
                case CSC:
                    input = sinl(input);
                    if (abs(input) <= EPSILON) {
                        cerr << "DOMAIN ERROR for CSC\n";
                        return ERR_RES;
                    }
                    num_stack.push(1 / input);
                    break;
                case SEC:
                    input = cosl(input);
                    if (abs(input) <= EPSILON) {
                        cerr << "DOMAIN ERROR for SEC\n";
                        return ERR_RES;
                    }
                    num_stack.push(1 / input);
                    break;
                case COT:
                    input = tanl(input);
                    if (abs(input) <= EPSILON) {
                        cerr << "DOMAIN ERROR for COT\n";
                        return ERR_RES;
                    }
                    num_stack.push(1 / input);
                    break;
                case ASIN:
                    input = asinl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;
                case ACOS:
                    input = acosl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;
                case ATAN:
                    input = atanl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;

                default:
                    cerr << "ERROR: Unknown error in Calc\n";
                    return ERR_RES;
            }
            // If a non-trig function (Since we already checked if the token is
            // a trig function prior, we don't need to here. Just don't change
            // the order of this if else)
        } else if (fn_set.find(tok) != fn_set.end()) {
            long double input = num_stack.top();
            num_stack.pop();
            switch (tok) {
                case ABS:
                    num_stack.push(abs(input));
                    break;
                case SQRT:
                    num_stack.push(sqrt(input));
                    break;
                case LOG:
                    num_stack.push(log10l(input));
                    break;
                case LN:
                    num_stack.push(logl(input));
                    break;
                case SINH:
                    input = sinhl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;
                case COSH:
                    input = coshl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;
                case TANH:
                    input = tanhl(input);
                    if (abs(input) <= EPSILON) input = 0;
                    num_stack.push(input);
                    break;

                default:
                    cerr << "Error: fn_set error\n";
                    return ERR_RES;
            }
        }
    }
    return (abs(num_stack.top()) <= EPSILON) ? 0 : num_stack.top();
}