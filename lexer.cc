#include "lexer.h"
#include <algorithm>
#include <string> // transform
#include <stack>

using std::stack;

// END, ERR, NUM, EQ, PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN
map<Token, string> tok_str_map = {
	{END, "END"}, {ERR, "ERR"}, {NUM, "NUM"}, {EQ, "EQ"}, {PLUS, "PLUS"}, {MIN, "MIN"}, {MULT, "MULT"}, {DIV, "DIV"}, {EXP, "EXP"}, 
	{LPAREN, "LPAREN"}, {RPAREN, "RPAREN"},
	{SIN, "SIN"}, {COS, "COS"}, {TAN, "TAN"}, {CSC, "CSC"}, {SEC, "SEC"}, {COT, "COT"},
	{ASIN, "ASIN"}, {ACOS, "ACOS"}, {ATAN, "ATAN"},
	{SINH, "SINH"}, {COSH, "COSH"}, {TANH, "TANH"},
	{PI, "PI"}, {E, "E"},
	{ABS, "ABS"}, {SQRT, "SQRT"}, {LOG, "LOG"}, {LN, "LN"},
	{MOD, "MOD"}, {ANS, "ANS"},
};
map<string, Token> str_tok_map = {
	{"END", END}, {"ERR", ERR}, {"NUM", NUM}, {"EQ", EQ}, {"PLUS", PLUS}, {"MIN", MIN}, {"MULT", MULT}, {"DIV", DIV}, {"EXP", EXP},
	{"LPAREN", LPAREN}, {"RPAREN", RPAREN},
	{"SIN", SIN}, {"COS", COS}, {"TAN", TAN}, {"CSC", CSC}, {"SEC", SEC}, {"COT", COT},
	{"ASIN", ASIN}, {"ACOS", ACOS}, {"ATAN", ATAN},
	{"PI", PI}, {"E", E},
	{"ABS", ABS}, {"SQRT", SQRT}, {"LOG", LOG}, {"LN", LN},
	{"SINH", SINH}, {"COSH", COSH}, {"TANH", TANH},
	{"MOD", MOD}, {"ANS", ANS},
};
map<char, Token> sym_tok_map = {
	{'=', EQ}, {'+', PLUS}, {'-', MIN}, {'*', MULT}, {'/', DIV}, {'^', EXP}, {'(', LPAREN}, {')', RPAREN}, {'%', MOD} 
};
map<Token, char> tok_sym_map = {
	{EQ, '='}, {PLUS, '+'}, {MIN, '-'}, {MULT, '*'}, {DIV, '/'}, {EXP, '^'}, {LPAREN, '('}, {RPAREN, ')'}
};

set<Token> const_set = { PI, E, ANS };

// keyword sets for the lexer to detect keyword strings 
set<string> kw_str_set = { 
	"SIN", "COS", "TAN", "CSC", "SEC", "COT", 
	"ASIN", "ACOS", "ATAN",
	"PI", "E", // PIE
	"ABS", "SQRT", "LOG", "LN",
	"SINH", "COSH", "TANH",
	"MOD", "ANS"
};

// Set of all functions
set<Token> fn_set = { 
	SIN, COS, TAN, CSC, SEC, COT,  
	ABS, SQRT, LOG, LN,
	SINH, COSH, TANH
};

// Set of all supported operators
set<Token> op_set = { PLUS, MIN, MULT, DIV, EXP, LPAREN, RPAREN, MOD };

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
			if (!ss.get(ch))
				break;
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

// void Lexer::condenseNegNums(){
// 	// Nothing should be done if there is only one, or no tokens in the list.
// 	if (lex_list.size() <= 1)
// 		return;
// 
// 	// Check the first item in the list
// 	LexItem lex = lex_list[0];
// 	LexItem next_lex = lex_list[1];
// 	if (lex.getToken() == MIN && 
// 	next_lex.getToken() == NUM){
// 		// Condense neg NUM token
// 		LexItem condensed = LexItem(NUM, "-" + std::to_string(next_lex.getVal()));
// 		// Eat old tokens 
// 		lex_list.erase(lex_list.begin(), lex_list.begin()+2);
// 		// Insert new token
// 		lex_list.insert(lex_list.begin(), condensed);
// 	}
// 
// 	// Check the rest of the items in the list 
// 	unsigned int idx = 1;
// 	while (idx < lex_list.size()-1){
// 		LexItem prev_lex = lex_list[idx-1];
// 		LexItem lex = lex_list[idx];
// 		LexItem next_lex = lex_list[idx+1];
// 		// If current token is -, previous token is an operator, and next token is a number...
// 
// 		// FIXME: Add support for multiple chained negative expressions, like 5-----5
// 		// If current token is - and
// 		if (lex.getToken() == MIN && 
// 		// previous token is an operator and
// 		op_set.find(prev_lex.getToken()) != op_set.end() &&
// 		// next token is a number
// 		next_lex.getToken() == NUM){
// 			// Condense to neg NUM token
// 			LexItem condensed = LexItem(NUM, "-" + std::to_string(next_lex.getVal()));
// 			// Eat the old tokens
// 			lex_list.erase(lex_list.begin()+idx, lex_list.begin()+idx+2);
// 			// Insert condensed token
// 			lex_list.insert(lex_list.begin()+idx, condensed);
// 			idx--;
// 		}
// 		idx++;
// 	}
// }

void Lexer::condenseNegNums(){
	// Nothing should be done if there is only one, or no tokens in the list.
	if (lex_list.size() <= 1)
		return;
	unsigned int idx = 1;
	LexItem prev;
	LexItem lex;

	while (idx < lex_list.size()){
		prev = lex_list[idx-1];
		lex = lex_list[idx];
		if (op_set.find(prev.getToken()) != op_set.end() && prev.getToken() != RPAREN && lex.getToken() == MIN){
			int sign = 1;
			while (lex.getToken() == MIN){
				sign *= -1;	
				lex_list.erase(lex_list.begin()+idx);
				if (idx == lex_list.size()){
					err_flag = true;
					cerr << "ERROR: - operator is missing a number.\n";
					return;
				}
				lex = lex_list[idx];
			}
			if (sign > 0)
				continue;
			lex_list.insert(lex_list.begin()+idx, LexItem(MULT));
			lex_list.insert(lex_list.begin()+idx, LexItem(NUM, "-1"));
		}
		idx++;
	}

	// Edge case for expression: --val
	if (lex_list[0].getToken() == MIN){
		lex_list.erase(lex_list.begin());
		lex_list.insert(lex_list.begin(), LexItem(MULT));
		lex_list.insert(lex_list.begin(), LexItem(NUM, "-1"));
	}
}

// Inserts MULT tokens where numbers are next to parenthesis
void Lexer::insertImplicitMult(){
	unsigned int idx = 0;
	while(idx < lex_list.size()-1){
		LexItem lex = lex_list[idx];
		LexItem next = lex_list[idx+1];
		// If current token is a number and
		if (lex.getToken() == NUM && 
		// if next token is left parenthesis or
		(next.getToken() == LPAREN ||
		// next token is a function 
		fn_set.find(next.getToken()) != fn_set.end()
		)){
			// Then insert a mult token
			lex_list.insert(lex_list.begin()+idx+1, LexItem(MULT));
		// If current token is a right parenthesis and
		} else if (lex.getToken() == RPAREN &&
		// next token is a number or a function
		(next.getToken() == NUM || fn_set.find(next.getToken()) != fn_set.end())){
			// Then insert a mult token
			lex_list.insert(lex_list.begin()+idx+1, LexItem(MULT));
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
	insertImplicitMult();
}

void Lexer::clear(){
	err_flag = false;
	ss.clear();
	lex_list.clear();
}
