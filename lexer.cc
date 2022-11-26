#include "lexer.h"
#include <algorithm>
#include <string> // transform
#include <stack>

using std::stack;

// END, ERR, NUM, EQ, PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN
map<Token, string> tok_str_map = {
	{END, "END"}, {ERR, "ERR"}, {NUM, "NUM"}, {EQ, "EQ"}, {PLUS, "PLUS"}, {MIN, "MIN"}, {MULT, "MULT"}, {DIV, "DIV"}, {EXP, "EXP"}, 
	{LPAREN, "LPAREN"}, {RPAREN, "RPAREN"},
	{SIN, "SIN"}, {COS, "COS"}, {TAN, "TAN"}, {CSC, "CSC"}, {SEC, "SEC"}, {COT, "COT"}
};
map<string, Token> str_tok_map = {
	{"END", END}, {"ERR", ERR}, {"NUM", NUM}, {"EQ", EQ}, {"PLUS", PLUS}, {"MIN", MIN}, {"MULT", MULT}, {"DIV", DIV}, {"EXP", EXP},
	{"LPAREN", LPAREN}, {"RPAREN", RPAREN},
	{"SIN", SIN}, {"COS", COS}, {"TAN", TAN}, {"CSC", CSC}, {"SEC", SEC}, {"COT", COT}
};
map<char, Token> sym_tok_map = {
	{'=', EQ}, {'+', PLUS}, {'-', MIN}, {'*', MULT}, {'/', DIV}, {'^', EXP}, {'(', LPAREN}, {')', RPAREN}
};
map<Token, char> tok_sym_map = {
	{EQ, '='}, {PLUS, '+'}, {MIN, '-'}, {MULT, '*'}, {DIV, '/'}, {EXP, '^'}, {LPAREN, '('}, {RPAREN, ')'}
};

// keyword sets for the lexer to detect keyword strings 
set<string> kw_str_set { "SIN", "COS", "TAN", "CSC", "SEC", "COT" };

// Set of all functions (currently this is the same as trig_set but more will be added)
set<Token> fn_set = { SIN, COS, TAN, CSC, SEC, COT };

// Set of all supported operators
set<Token> op_set = { PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN };

ostream& operator<<(ostream& out, LexItem& lex){
	Token tok = lex.getToken();
	out << lex.getTokenStr();
	if (tok == NUM)
		out << "(" << lex.getVal() << ")";
	return out;
}


LexItem Lexer::getNextToken(){
	char ch;
	do { 
		if (!ss.get(ch))
			return LexItem(END);
	} while (isspace(ch));

	if (sym_tok_map.find(ch) != sym_tok_map.end()){
		return LexItem(sym_tok_map[ch], string(ch, 1));
	} else if (ch == '.' || isdigit(ch)){
		string lexeme;
		bool has_dot = false;
		while (ch == '.' || isdigit(ch)){
			lexeme += ch;
			if (ch == '.'){
				if(has_dot){
					cerr << "ERROR: Number \"" << lexeme << "\" contains an illegal decimal." << endl;
					err_flag = true;
					return LexItem(ERR);
				} else {
					has_dot = true;
				}
			}
			if (!ss.get(ch))
				return LexItem(NUM, lexeme);
		}
		ss.putback(ch);
		return LexItem(NUM, lexeme);
	} else if (isalpha(ch)){ // search for keyword
		// Gather preceeding chars into a string
		string kw;
		do {
			kw += ch;
			if (!ss.get(ch)){
				cerr << "ERROR: Invalid keyword, or missing function paremeter \"" << kw << "\"\n";
				err_flag = true;
				return LexItem(ERR);
			}
		} while (isalpha(ch));
		ss.putback(ch);
		// Keywords are case-insensitive, convert to uppercase
		std::transform(kw.begin(), kw.end(), kw.begin(), ::toupper);
		// Search kw_str_set for a match
		if (kw_str_set.find(kw) != kw_str_set.end()){
			// cout << "Found keyword \"" << kw << "\"\n";
			Token tok = str_tok_map[kw];
			return LexItem(tok, kw);
		}
		
		// No match error
		cerr << "ERROR: Unknown keyword \"" << kw << "\"\n";
		err_flag = true;
		return LexItem(ERR);
	}

	cerr << "ERROR: Illegal character \"" << ch << "\"" << endl;
	err_flag = true;
	return LexItem(ERR);
}

void Lexer::condenseNegNums(){
	// Nothing should be done if there is only one, or no tokens in the list.
	if (lex_list.size() <= 1)
		return;

	// Check the first item in the list
	LexItem lex = lex_list[0];
	LexItem next_lex = lex_list[1];
	if (lex.getToken() == MIN && 
	next_lex.getToken() == NUM){
		// Condense neg NUM token
		LexItem condensed = LexItem(NUM, "-" + std::to_string(next_lex.getVal()));
		// Eat old tokens 
		lex_list.erase(lex_list.begin(), lex_list.begin()+2);
		// Insert new token
		lex_list.insert(lex_list.begin(), condensed);
	}

	// Check the rest of the items in the list 
	int idx = 1;
	int end = lex_list.size()-1;
	while (idx < end){
		LexItem prev_lex = lex_list[idx-1];
		LexItem lex = lex_list[idx];
		LexItem next_lex = lex_list[idx+1];
		// If current token is -, previous token is an operator, and next token is a number...
		if (lex.getToken() == MIN && 
		std::find(op_set.begin(), op_set.end(), prev_lex.getToken()) != op_set.end() &&
		next_lex.getToken() == NUM){
			// Condense to neg NUM token
			LexItem condensed = LexItem(NUM, "-" + std::to_string(next_lex.getVal()));
			// Eat the old tokens
			lex_list.erase(lex_list.begin()+idx, lex_list.begin()+idx+2);
			// Insert condensed token
			lex_list.insert(lex_list.begin()+idx, condensed);
			idx--;
		}
		idx++;
	}
}

void Lexer::gatherLexemes(){
	LexItem lex;
	while ( ((lex = getNextToken()).getToken() != END) &&
	(lex.getToken() != ERR))
		pushBackLex(lex);
	condenseNegNums();
}

void Lexer::clear(){
	err_flag = false;
	ss.clear();
	lex_list.clear();
}
