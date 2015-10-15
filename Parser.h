#pragma once
#include "Tokenizer.h"
#include "Expression.h"
#include "ExprBinOp.h"
#include "ExprIntConst.h"
#include "ExprRealConst.h"
#include "ExprVar.h"

#define MaxPriority 5
#define ParseF (MaxPriority + 1)
enum {AssignP, PlusMinusP, MulDivP, FuncP, ArrP, RecP};

class Parser
{	
private:
	Tokenizer T;
	Expression* root;
	double Calc(Expression* node);
	void AddVar(double val, string name);
	bool CheckSign(Lexeme S);
	bool CheckOperand(Lexeme S);
	int numVars;
	string* names;           
	double* values;
	Lexeme SaveLex;
public:
	static const int priority_length[6];
	static const int priority_op[6][2];
	double CalcTree();
	void Parse();
	void PrintTree(string FileName);
	void PrintNode(Expression* node, int h, FILE* F);
	Expression* ParseExpr(Expression* left, int priority);
	Expression* ParseFactor();
	Expression* Brackets();
	Parser(const char* FileName);
	Parser();
	~Parser();
};

