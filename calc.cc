#include "calc.h"

float Calc::evalRPN(queue<LexItem> output_queue){
    while (!output_queue.empty()){
        LexItem lex = output_queue.front();
        Token tok = lex.getToken();
        output_queue.pop();
        if (tok == NUM){
            num_stack.push(lex.getVal());
        } else {
            // If the num_stack is empty for any of these calls, there must be a syntax error somewhere.
            assert(!num_stack.empty());
            float b = num_stack.top(); num_stack.pop();
            assert(!num_stack.empty());
            float a = num_stack.top(); num_stack.pop();
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
                    assert(b != 0);
                    num_stack.push(a / b);
                    break;
                case EXP:
                    num_stack.push(powf(a, b));
                    break;
            }
        }
    }
    return num_stack.top();
}