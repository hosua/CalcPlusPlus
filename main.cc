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

		if (!parser->checkConsecutiveOpsAndNums() || !parser->validateFunctionParenthesis()){
			cerr << "Parser detected an error, please check your syntax.\n";
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
	string eq_str = "";
	cout << "--------------------------------------------------\n" <<
	"By default, the calculator is set to degrees mode.\n" <<
	"If you wish to change this, type !deg or !rad to change modes.\n" <<
	"--------------------------------------------------\n";
	cout << "Enter a math expression (press Ctrl+c to quit):\n";
	while (getline(cin, eq_str)){
		// Check if there is a command
		if (eq_str.size() == 0){
			cerr << "ERROR: You must enter an expression or a command!\n";
			continue;
		}
		if (eq_str[0] == '!'){
			if (eq_str.size() == 1){
				cerr << "ERROR: The command name must follow '!'\n";
				continue;
			}
			unsigned int idx = 1;
			string cmd = "";
			while (idx < eq_str.size())
				cmd += eq_str[idx++];
			// No case sensitivity
			std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
			// Handle commands
			if (cmd == "DEG" || cmd == "DEGREE" || cmd == "DEGREES")
				calc->setDegreeRadian("degree");
			else if (cmd == "RAD" || cmd == "RADIAN" || cmd == "RADIANS")
				calc->setDegreeRadian("radian");
			else
				cerr << "Invalid command \"" << cmd << "!\"\n";
		} else {
			long double res = evalEquationStr(lexer, parser, calc, eq_str);
			if (res != ERR_RES) 
				cout << std::setprecision(64) << res << "\n";
			calc->setPrevAns(res);
		}
		cout << "Enter a math expression (press Ctrl+c to quit):\n";
	}

	delete parser;
	delete lexer;
	delete calc;
}
