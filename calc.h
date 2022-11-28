#ifndef CALC_H
#define CALC_H

#include "lexer.h"
#include <queue>
#include <stack>
#include <math.h>

using std::queue;
using std::stack;

// Any errors will arbritrarily set the value to the min of a long double
#define ERR_RES std::numeric_limits<long double>::min()

// Used for checking if a number is close enough to zero to be considered zero.
#define EPSILON std::numeric_limits<long double>::epsilon() * 10

#define EULERS_NUM exp(1)

// Set of all trig functions
extern set<Token> trig_set;

class Calc {
public:
    Calc() : radians_mode(false), prev_ans(0) {}
    ~Calc(){}
    // After the parser converts the lex list into Reverse-Polish Notation, it can be evaluated with this function.
    long double evalRPN(queue<LexItem> output_queue);

    // TODO: Implement ability to change degree/radian mode
    bool isRadianMode(){ return radians_mode; }
    void toggleRadianMode(){
        (radians_mode) ? radians_mode = false : radians_mode = true;
    }

	long double getPrevAns(){ return prev_ans; }
	void setPrevAns(long double ans){ prev_ans = ans; }

private:
    bool radians_mode;
    long double convertToRadians(float num){ return num * (M_PI/180); }
    stack<long double> num_stack;
    stack<Token> op_stack;

	long double prev_ans;
};

#endif // CALC_H