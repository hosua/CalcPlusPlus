#include "lexer.h"

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
	exit(EXIT_FAILURE);
}

void Lexer::condenseNegNums(){
	int idx = 1;
	int end = lex_list.size()-2;

	// LUL
	while (idx < end){
		LexItem lex = lex_list[idx];
		Token tok = lex.getToken();
		if (tok == MIN){
			LexItem prev = lex_list[idx-1];
			LexItem next = lex_list[idx+1];
			LexItem next_next = lex_list[idx+2];
			if (prev.getToken() == LPAREN && next.getToken() == NUM && next_next.getToken() == RPAREN){
				LexItem new_lex = LexItem(NUM, "-" + std::to_string(next.getVal()));
				lex_list.erase(lex_list.begin()+idx-1, lex_list.begin()+idx+3);
				lex_list.insert(lex_list.begin()+idx-1, new_lex);
				idx--;
			}
		}
		idx++;
	}
}

void Lexer::gatherLexemes(){
	LexItem lex;
	while ( (lex = getNextToken()).getToken() != END)
		pushBackLex(lex);
	condenseNegNums();
}
