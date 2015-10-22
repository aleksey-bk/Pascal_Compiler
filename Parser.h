#pragma once
#include "Tokenizer.h"
#include "Expression.h"
#include "ExprBinOp.h"
#include "ExprIntConst.h"
#include "ExprRealConst.h"
#include "ExprVar.h"
#include "ExprArgs.h"
#include "ExprUnOp.h"
#include <vector>

#define MaxPriority 3
#define ParseF (MaxPriority + 1)
enum {FormNoBr, FormExpr, FormFunc, FormArr, FormUnMinus};

class Parser
{	
private:
	Tokenizer T;
	Expression* root;
	Expression* ParseExpr(Expression* left, int form, int priority);
	Expression* ParseFactor(int form);
	Expression* ParseArgs(int id_end_parse);
	void CheckSign(Lexeme S, int form);
	void CheckOperand(Lexeme S);
	void CheckLeft(Lexeme S, Expression* left);
	void CheckRight(Lexeme S, Expression* right);
	exception GetErrInformation(Lexeme l, string inf);
	bool dot_com_flag;
	bool IsTypename(Lexeme l);
	int IsUnaryOp(Lexeme l);
	bool ParseArgsStatus;
	int numVars;
	string* names;           
	double* values;
	Lexeme SaveLex;
	double Calc(Expression* node);
	void AddVar(double val, string name);
	void PrintNode(Expression* node, int h, FILE* F);
public:
	static const int priority_length[MaxPriority + 1];
	static const int priority_op[MaxPriority + 1][3];
	double CalcTree();
	void Parse();
	void PrintTree(string FileName);
	Parser(const char* FileName);
	Parser();
	~Parser();
};

