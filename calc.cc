#include "calc.h"
#include <math.h>
#include <limits>

double Calc::evalRPN(queue<LexItem> output_queue){
    while (!output_queue.empty()){
        LexItem lex = output_queue.front();
        Token tok = lex.getToken();
        output_queue.pop();
        if (tok == NUM){
            num_stack.push(lex.getVal());
        // If token is a trig function
        } else if (trig_set.find(tok) == trig_set.end()) {
            // If the num_stack is empty for any of these calls, there must be a syntax error somewhere.
            if(num_stack.empty()){
                cerr << "ERROR: Calc detected syntax error\n";
                return ERR_RES;
            }
            double b = num_stack.top(); num_stack.pop();
            assert(!num_stack.empty());
            double a = num_stack.top(); num_stack.pop();
            switch(tok) {
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
                    if (abs(b) <= EPSILON){
                        cerr << "DIVIDE BY ZERO ERROR\n";
                        return ERR_RES; 
                    }
                    num_stack.push(a / b);
                    break;
                case EXP:
                    num_stack.push(powf(a, b));
                    break;
            }
        } else {
            double a = convertToRadian(num_stack.top()); 
            num_stack.pop();
            // FIXME: Order of operations for functions are not yet working properly. 
            // I think the best solution is to have the lexer insert parenthesis when
            // it does not find any after a function keyword.
            switch(tok){
                case SIN:
                    num_stack.push(sin(a));
                    break;
                case COS:
                    // This needs to be done to avoid floating-point rounding errors
                    a = cos(a);
                    if (abs(a) <= EPSILON)
                        a = 0;
                    num_stack.push(a);
                    break;
                case TAN:
                    if (std::fmod(abs(a), M_PI_2) == 0){
                        cerr << "DOMAIN ERROR for TAN\n";
                        return ERR_RES;
                    }
                    num_stack.push(tan(a));
                    break;
                case CSC:
                    a = sin(a);
                    if (abs(a) <= EPSILON){
                        cerr << "DOMAIN ERROR for CSC\n";
                        return ERR_RES;
                    }
                    num_stack.push(1/sin(a));
                    break;
                case SEC:
                    a = cos(a);
                    if (abs(a) <= EPSILON){
                        cerr << "DOMAIN ERROR for SEC\n";
                        return ERR_RES;
                    }
                    num_stack.push(1/cos(a));
                    break;
                case COT:
                    a = tan(a);
                    if (abs(a) <= EPSILON){
                        cerr << "DOMAIN ERROR for COT\n";
                        return ERR_RES;
                    }
                    num_stack.push(1/tan(a));
                    break;
            }
        }
    }
    return (abs(num_stack.top()) <= EPSILON) ? 0 : num_stack.top();
}