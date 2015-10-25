#pragma once
#include "Expression.h"
#include "Lexeme.h"

class ExprBinOp : public Expression
{
public:
	int operation;
	ExprBinOp(Expression* L, int t, Expression* R);
	~ExprBinOp();
};

