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
#define IsSimpleOp ((priority == 1 || priority == 2))
#define ParseF (MaxPriority + 1)

class Parser
{	
	friend class ExprUnOp;
private:
	Tokenizer T;
	Lexeme LastLex;
	Lexeme SaveLex;
	Expression* root;
	Expression* ParseExpr(int priority, Expression* L);
	Expression* ParseFactor();
	Expression* ParseAssign(Expression* l);
	Expression* ParseRecord(Expression* l);
	Expression* ParseFunction(int& operation_type, Expression* l);
	Expression* ParseArray(Expression* l);
	Expression* ParseCast();
	Expression* ParseBinOp(int op_idx, int priority);
	exception GetErrInformation(Lexeme l, string inf);
	void CheckSign(Lexeme S);
	void CheckOperand(Lexeme S);
	bool dot_com_flag;
	bool IsTypename(string s);
	int IsUnaryOp(Lexeme l);
	bool ParseArgsStatus;
	void PrintNode(Expression* node, int h, FILE* F);
public:
	static const int priority_length[MaxPriority + 1];
	static const int priority_op[MaxPriority + 1][5];
	void Parse();
	void PrintTree(string FileName);
	Parser(const char* FileName);
	Parser();
	~Parser();
};



