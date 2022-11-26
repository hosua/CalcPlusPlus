#include "lexer.h"
#include "parser.h"
#include "calc.h"
#include <limits>

double evalEquationStr(Lexer* lexer, Parser* parser, Calc* calc, string eq_str){
	lexer->setEquationStr(eq_str);
	lexer->gatherLexemes();
	// lexer->printLexList(); // For debugging
	vector<LexItem> lex_list = lexer->getLexList();

	double res; 
	if (!lexer->checkErr()){
		parser->setLexList(lex_list);
		parser->convertToRPN();

		queue<LexItem> output_queue = parser->getOutputQueue();
		res = calc->evalRPN(output_queue);
	} else {
		cerr << "Could not calculate that expression. Please check your syntax.\n";
		res = ERR_RES;
	}

	lexer->clear();
	parser->clear();
	return res;
}

// TODO: Impelement CLI to use readline and history libraries

int main(){
	Parser* parser = new Parser();
	Lexer* lexer = new Lexer();
	Calc* calc = new Calc();
	string eq_str;
	cout << "Enter a math expression:\n";
	while (getline(cin, eq_str)){
		double res = evalEquationStr(lexer, parser, calc, eq_str);
		if (res != ERR_RES)
			cout << res << "\n";
		cout << "Enter a math expression:\n";
	}

	delete parser;
	delete lexer;
	delete calc;
}
