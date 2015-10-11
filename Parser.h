#pragma once
#include "Tokenizer.h"
#include "Expression.h"
#include "ExprBinOp.h"
#include "ExprIntConst.h"
#include "ExprRealConst.h"
#include "ExprVar.h"

class Parser
{	
private:
	Tokenizer T;
	Expression* root;
	void DestroyNode(Expression* node);
	double Calc(Expression* node);
	void AddVar(double val, string name);
	bool CheckSign(Lexeme S);
	bool CheckOperand(Lexeme S);
	int numVars;
	string* names;           
	double* values;
	Lexeme SaveLex;
	bool except;
	int BracketCounter;
public:
	double CalcTree();
	void Parse();
	void PrintTree(string FileName);
	void PrintNode(Expression* node, int h, FILE* F);
	bool getException();
	Expression* ParseExpr(Expression* left);
	Expression* ParseTerm(Expression* left);
	Expression* ParseFactor();
	Expression* Brackets();
	Parser(const char* FileName);
	~Parser();
	const bool FileNotFound;
};

