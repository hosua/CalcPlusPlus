#include "parser.h"

map<string, vector<Token>> assoc_map = {
	{"left", {MULT, DIV, PLUS, MIN, MOD}},
	{"right", {EXP}}
};

map<Token, int> op_precedence_map = {
	{EXP, 3},
    {MOD, 2},
	{MULT, 2},
	{DIV, 2},
	{PLUS, 1},
	{MIN, 1},
};

// See: https://en.wikipedia.org/wiki/Shunting_yard_algorithm
// Converts Lex list into Reverse-Polish Notation form using the shunting yard algorithm
void Parser::convertToRPN(){
    for (LexItem lex : lex_list){
        LexItem op_1 = lex;
        Token tok_1 = op_1.getToken();
        // - a number or a constant
        if (tok_1 == NUM || const_set.find(tok_1) != const_set.end()){
            output_queue.push(op_1);
        // - a function
        } else if (fn_set.find(tok_1) != fn_set.end()){
            op_stack.push(op_1);
        // - an operator op_1:
        } else if (tok_1 != LPAREN && tok_1 != RPAREN && op_set.find(tok_1) != op_set.end()){
            vector<Token> left_assoc = assoc_map["left"];
            LexItem op_2;
            // while...
            while ( !op_stack.empty() && 
            // there is an operator op_2 other than the left parenthesis at the top of the operator stack, 
            (op_2 = op_stack.top()).getToken() != LPAREN &&
            // and (op_2 has greater precedence than op_1
            ((op_precedence_map[op_2.getToken()] > op_precedence_map[tok_1] || 
            // or they have the same precedence
            (op_precedence_map[op_2.getToken()] == op_precedence_map[tok_1] && 
            // and op_1 is left-associative)
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
                if (op_stack.empty()){
				    cerr << "ERROR: Parenthesis mismatch \")\".\n";
                    err_flag = true;
                    return;
                }
                op_stack.pop();
                output_queue.push(op_2);
            }
            // If there is no parenthesis here, there is a mismatch.
            if (op_stack.top().getToken() != LPAREN){
				cerr << "ERROR: Parenthesis mismatch \")\".\n";
                err_flag = true;
                return;
            }
            // discard LPAREN
            op_stack.pop();

            if(!op_stack.empty()){
                op_2 = op_stack.top();
                Token tok_2 = op_2.getToken();
                // if there is a function on top
                if (fn_set.find(tok_2) != fn_set.end()){
                    // pop it into the output queue
                    op_stack.pop();
                    output_queue.push(op_2);
                }
            }
        }
    }

    while (!op_stack.empty()){
        // If the operator token on the top of the stack is a left parenthesis, then there is a mismatch.
        if (op_stack.top().getToken() == LPAREN){
			cerr << "ERROR: Parenthesis mismatch \"(\".\n";
            err_flag = true;
            return;
        }

        LexItem op = op_stack.top();
        output_queue.push(op);
        op_stack.pop();
    }
    /* DEBUGGING */
    // cout << "-----" << output_queue.size() << "------" << endl;
    // while (!output_queue.empty()){
    //     cout << output_queue.front() << endl;
    //     output_queue.pop();
    // }
    // exit(0);
}

// Checks lex_list and ensures that all functions are preceded by an LPAREN
bool Parser::validateFunctionParenthesis(){
    for (int idx = 0; idx < (int)(lex_list.size()-1); idx++){
        LexItem lex = lex_list[idx];
        if (fn_set.find(lex.getToken()) != fn_set.end()){
            LexItem next = lex_list[idx+1];
            if (next.getToken() != LPAREN){
                err_flag = true;
                cerr << "ERROR: Input paremeter for function \"" << tok_str_map[lex.getToken()] << "\" must be wrapped in parenthesis.\n";
                return false;
            }
        }
    }
    return true;
}

// Check lex_list for consecutive operators or numbers, which are syntax errors
bool Parser::checkConsecutiveOpsAndNums(){
    unsigned int idx = 0;
    while (idx < lex_list.size()-1){
        LexItem lex = lex_list[idx];
        LexItem next = lex_list[idx+1];
        if (op_set.find(lex.getToken()) != op_set.end() &&
        op_set.find(next.getToken()) != op_set.end() &&
        next.getToken() != LPAREN &&
        lex.getToken() != RPAREN){
            cerr << "ERROR: Detected consecutive operators: \"" 
            << tok_sym_map[lex.getToken()] << "\" and \"" 
            << tok_sym_map[next.getToken()] << "\"\n";
            return false;
        } else if (lex.getToken() == NUM && next.getToken() == NUM){
            cerr << "ERROR: Detected consecutive numbers: \"" << lex.getVal() << "\" and \"" << next.getVal() << "\".\n";
            return false;
        }
        idx++;
    }
    return true;
}

void Parser::clear(){
    err_flag = false;
    lex_list.clear();
    // Swap op_stack with empty_stack to clear it
    stack<LexItem> empty_stack;
    std::swap(op_stack, empty_stack);
    // Swap output_queue with empty_queue to clear it
    queue<LexItem> empty_queue;
    std::swap(output_queue, empty_queue);
}