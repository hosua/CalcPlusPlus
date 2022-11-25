#include "lexer.h"
#include "parser.h"
#include "calc.h"

float evalEquationStr(string eq_str){
	cout << "eq_str: " << eq_str << endl;
	Parser parser;
	Lexer lexer;
	Calc calc;

	lexer.setEquationStr(eq_str);
	lexer.gatherLexemes();
	lexer.printLexList(); 
	vector<LexItem> lex_list = lexer.getLexList();

	parser.setLexList(lex_list);
	parser.convertToRPN();

	queue<LexItem> output_queue = parser.getOutputQueue();
	return calc.evalRPN(output_queue);
}

int main(){
	string eq_str = "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3";
	// string eq_str = "3 + 4 * (2 - 1)";
	float res = evalEquationStr(eq_str);
	cout << "Result: " << res << endl;
}
