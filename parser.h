#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

#include <set>
#include <stack>
#include <queue>
#include <algorithm>

using std::vector;
using std::stack;
using std::queue;

extern map<string, vector<Token>> assoc_map;
extern map<Token, int> op_precedence_map;

class Parser {
public:
    Parser() : err_flag(false) {}
    Parser(vector<LexItem> lex_list) : lex_list(lex_list){}
    ~Parser(){}

    // Checks lex_list and ensures that all functions are preceded by an LPAREN
    bool validateFunctionParenthesis();
    // Check lex_list for consecutive operators or numbers
    bool checkConsecutiveOpsAndNums();
    bool checkErr(){ return err_flag; }
    void setLexList(vector<LexItem> lex_list){ this->lex_list = lex_list; }
    // Convert to Reverse-Polish Notation and store in output_queue
    void convertToRPN(); 
    // After converting the lexeme list into Reverse-Polish Notation, the output queue can now be evaluated.
    queue<LexItem> getOutputQueue(){ return output_queue; }
    // Clear everything in the parser
    void clear();

private:
    bool err_flag;
    vector<LexItem> lex_list;
    stack<LexItem> op_stack;
    queue<LexItem> output_queue;
};

#endif // PARSER_H
