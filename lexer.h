#ifndef LEXER_H
#define LEXER_H
#include <assert.h>
#include <iostream>
#include <limits.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <sstream>
#include <vector>

using std::ostream;
using std::string;
using std::stringstream;
using std::set;
using std::map;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;

enum Token {
	END, ERR, NUM, EQ, PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN
};
extern map<Token, string> tok_str_map;
extern map<string, Token> str_tok_map;
extern map<char, Token> sym_tok_map;
extern map<Token, char> tok_sym_map;
extern set<Token> op_set;

class LexItem {
private:
	Token tok;	
	string lex_str;
	float val;
public:
	LexItem() : val(INT_MIN){}
	LexItem(Token tok, string lexeme) : val(INT_MIN), tok(tok){
		if (tok == NUM)
			val = std::stof(lexeme);
	}
	~LexItem(){}

	Token getToken(){ return tok; } 
	string getTokenStr(){ return tok_str_map[tok]; } 
	float getVal();
};
// Allow LexItem to be printed directly
extern ostream& operator<<(ostream& out, LexItem& lex);

class Lexer {
private:
	stringstream ss;
	vector<LexItem> lex_list;

	// Gets the next token in the string stream
	LexItem getNextToken();
	// After getting all tokens, this function will "condense" all negative numbers into negative tokens. (i.e., (-42) -> -42)
	void condenseNegNums();

public:
	Lexer(){}
	Lexer(string eq_str){ ss << eq_str; }
	~Lexer(){}
	void setEquationStr(string eq_str){ ss << eq_str; }
	void gatherLexemes();

	void pushBackLex(LexItem lex){ lex_list.push_back(lex); }
	void printLexList(){
		for (LexItem lex : lex_list)
			cout << lex << endl;
	}
	vector<LexItem> getLexList(){ return lex_list; }
	// Clear everything in the lexer
	void clear();
};



#endif // LEXER_H
