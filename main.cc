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
	// lexer.printLexList(); 
	vector<LexItem> lex_list = lexer.getLexList();

	parser.setLexList(lex_list);
	parser.convertToRPN();

	queue<LexItem> output_queue = parser.getOutputQueue();
	return calc.evalRPN(output_queue);
}

int main(){
	// string eq_str = "420 * 69 / 24.5 * (6^2) + 4";
	// string eq_str = "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3";
	// string eq_str = "3 + 4 * (2 - 1)";
	// string eq_str = "(2+3/2)/(6*4+2+3)";
	string eq_str;
	cout << "Enter a math expression:\n";
	while (getline(std::cin, eq_str)){
		float res = evalEquationStr(eq_str);
		cout << "Result: " << res << endl
			<< "Enter a math expression:\n";
	}
}
