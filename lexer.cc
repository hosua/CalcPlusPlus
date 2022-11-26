#include "lexer.h"
#include <algorithm>

// END, ERR, NUM, EQ, PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN
map<Token, string> tok_str_map = {
	{END, "END"}, {ERR, "ERR"}, {NUM, "NUM"}, {EQ, "EQ"}, {PLUS, "PLUS"}, {MIN, "MIN"}, {MULT, "MULT"}, {DIV, "DIV"}, {EXP, "EXP"},
	{LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}
};
map<string, Token> str_tok_map = {
	{"END", END}, {"ERR", ERR}, {"NUM", NUM}, {"EQ", EQ}, {"PLUS", PLUS}, {"MIN", MIN}, {"MULT", MULT}, {"DIV", DIV}, {"EXP", EXP},
	{"LPAREN", LPAREN}, {"RPAREN", RPAREN}
};
map<char, Token> sym_tok_map = {
	{'=', EQ}, {'+', PLUS}, {'-', MIN}, {'*', MULT}, {'/', DIV}, {'^', EXP}, {'(', LPAREN}, {')', RPAREN}
};
map<Token, char> tok_sym_map = {
	{EQ, '='}, {PLUS, '+'}, {MIN, '-'}, {MULT, '*'}, {DIV, '/'}, {EXP, '^'}, {LPAREN, '('}, {RPAREN, ')'}
};

// Set of all supported operators
set<Token> op_set = { PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN };

ostream& operator<<(ostream& out, LexItem& lex){
	Token tok = lex.getToken();
	out << lex.getTokenStr();
	if (tok == NUM)
		out << "(" << lex.getVal() << ")";
	return out;
}

float LexItem::getVal(){
	// A value should never be retrived if the token is an operator.
	assert(tok == NUM);
	return val;
}


LexItem Lexer::getNextToken(){
	char ch;
	do { 
		if (!ss.get(ch))
			return LexItem(END, "END");
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
					exit(EXIT_FAILURE);
				} else {
					has_dot = true;
				}
			}
			if (!ss.get(ch))
				return LexItem(NUM, lexeme);
		}
		ss.putback(ch);
		return LexItem(NUM, lexeme);
	}

	cerr << "ERROR: Illegal character \"" << ch << "\"" << endl;
	return LexItem(ERR, "ERR");
}

// TODO: This is probably doable in the first pass of the lexer, but might make things more messy. Consider trying implementing a one-pass solution.
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
	ss.clear();
	lex_list.clear();
}
