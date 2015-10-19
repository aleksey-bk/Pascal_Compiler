#pragma once
#include "Expression.h"
class ExprUnOp : public Expression
{
public:
	ExprUnOp(Expression* left, int t, Expression* right);
	~ExprUnOp();
};

