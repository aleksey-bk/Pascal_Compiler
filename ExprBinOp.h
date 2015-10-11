#pragma once
#include "Expression.h"
#include "Lexeme.h"

class ExprBinOp : public Expression
{
public:
	ExprBinOp(Expression* L, int t, Expression* R);
	~ExprBinOp();
};

