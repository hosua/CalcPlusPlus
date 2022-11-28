#include "lexer.h"
#include "parser.h"
#include "calc.h"
#include <limits>
#include <iomanip>

long double evalEquationStr(Lexer* lexer, Parser* parser, Calc* calc, string eq_str){
	lexer->setEquationStr(eq_str);
	lexer->gatherLexemes();
	// lexer->printLexList(); // For debugging
	long double res; 
	vector<LexItem> lex_list = lexer->getLexList();

	if (!lexer->checkErr()){
		parser->setLexList(lex_list);
		if (!parser->validateFunctionParenthesis()){
			res = ERR_RES;
		} else {
			parser->convertToRPN();
			if (parser->checkErr()){
				cerr << "Parser detected an error, please check your syntax.\n";
				res = ERR_RES;
			} else {
				queue<LexItem> output_queue = parser->getOutputQueue();
				res = calc->evalRPN(output_queue);
			}
		}
	} else {
		cerr << "Lexer detected an error, please check your syntax.\n";
		res = ERR_RES;
	}

	lexer->clear();
	parser->clear();
	return res;
}

// TODO: (maybe) Impelement CLI to use readline and history libraries

int main(){
	Parser* parser = new Parser();
	Lexer* lexer = new Lexer();
	Calc* calc = new Calc();
	string eq_str;
	cout << "Enter a math expression (press Ctrl+c to quit):\n";
	while (getline(cin, eq_str)){
		long double res = evalEquationStr(lexer, parser, calc, eq_str);
		(res != ERR_RES) ? cout << std::setprecision(64) << res << "\n" : cerr << "Calc error.\n";
		cout << "Enter a math expression (press Ctrl+c to quit):\n";
	}

	delete parser;
	delete lexer;
	delete calc;
}
