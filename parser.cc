#include "parser.h"

map<string, vector<Token>> assoc_map = {
	{"left", {MULT, DIV, PLUS, MIN}},
	{"right", {EXP}}
};

map<Token, int> op_precedence_map = {
	{EXP, 3},
	{MULT, 2},
	{DIV, 2},
	{PLUS, 1},
	{MIN, 1},
};

// non-parenthesis operators
set<Token> op_set = { PLUS, MIN, MULT, DIV, EXP };

// See: https://en.wikipedia.org/wiki/Shunting_yard_algorithm
// Converts Lex list into Reverse-Polish Notation form using the shunting yard algorithm
void Parser::convertToRPN(){
    for (LexItem lex : lex_list){
        LexItem op_1 = lex;
        Token tok_1 = op_1.getToken();
        if (tok_1 == NUM)
            output_queue.push(op_1);
        // - an operator op_1:
        else if (op_set.find(tok_1) != op_set.end()){
            vector<Token> left_assoc = assoc_map["left"];
            LexItem op_2;
            // while:
            //     there is an operator op_2 other than the left parenthesis at the top
            //     of the operator stack, and (op_2 has greater precedence than op_1
            //     or they have the same precedence and op_1 is left-associative)
            //
            while ( !op_stack.empty() && 
            (op_2 = op_stack.top()).getToken() != LPAREN &&
            ((op_precedence_map[op_2.getToken()] > op_precedence_map[tok_1] || 
            (op_precedence_map[op_2.getToken()] == op_precedence_map[tok_1] && 
            (std::find(left_assoc.begin(), left_assoc.end(), tok_1) != left_assoc.end()))))){
                // pop op_2 from stack
                op_stack.pop();
                output_queue.push(op_2);
            }
            // push op_1 onto the operator stack            
            op_stack.push(op_1);
        } else if (tok_1 == LPAREN){
            op_stack.push(op_1);
        } else if (tok_1 == RPAREN){
            LexItem op_2;
            while( (op_2 = op_stack.top()).getToken() != LPAREN){
                assert(!op_stack.empty());
                op_stack.pop();
                output_queue.push(op_2);
            }
            // If there is no parenthesis here, there is a mismatch.
            assert(op_stack.top().getToken() == LPAREN);
            // discard LPAREN
            op_stack.pop();
            // TODO: If I add functions like sin, cos, more code needs to go here.
        }
    }

    while (!op_stack.empty()){
        // If the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses.
        assert(op_stack.top().getToken() != LPAREN);
        LexItem op = op_stack.top();
        output_queue.push(op);
        op_stack.pop();
    }
    /* DEBUGGING FUNCTION */
    // while (!output_queue.empty()){
    //     cout << output_queue.front() << endl;
    //     output_queue.pop();

    // }
}