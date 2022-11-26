#include "lexer.h"
#include "parser.h"
#include "calc.h"

float evalEquationStr(Lexer* lexer, Parser* parser, Calc* calc, string eq_str){
	// cout << "eq_str: " << eq_str << endl;

	lexer->setEquationStr(eq_str);
	lexer->gatherLexemes();
	// lexer->printLexList(); // For debugging
	vector<LexItem> lex_list = lexer->getLexList();

	parser->setLexList(lex_list);
	parser->convertToRPN();

	queue<LexItem> output_queue = parser->getOutputQueue();

	float res = calc->evalRPN(output_queue);
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
		float res = evalEquationStr(lexer, parser, calc, eq_str);
		cout << res << "\n"
		<< "Enter a math expression:\n";
	}

	delete parser;
	delete lexer;
	delete calc;
}
