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
	Expression* root;
	bool dot_com_flag;
	double Calc(Expression* node);
	void AddVar(double val, string name);
	void CheckSign(Lexeme S, int form);
	void CheckOperand(Lexeme S);
	void CheckLeft(Lexeme S, Expression* left);
	bool ParseArgsStatus;
	int numVars;
	int func_br;
	int arr_br;
	string* names;           
	double* values;
	Lexeme SaveLex;
	Lexeme LastLex;
	exception GetErrInformation(Lexeme l, string inf);
public:
	Tokenizer T;
	static const int priority_length[MaxPriority + 1];
	static const int priority_op[MaxPriority + 1][3];
	double CalcTree();
	void Parse();
	void PrintTree(string FileName);
	void PrintNode(Expression* node, int h, FILE* F, bool args);
	bool IsTypename(Lexeme l);
	int IsUnaryOp(Lexeme l);
	Expression* ParseExpr(Expression* left, int form, int priority);
	Expression* ParseFactor(int form);
	Expression* ParseArgs(int id_end_parse);
	Parser(const char* FileName);
	Parser();
	~Parser();
};

