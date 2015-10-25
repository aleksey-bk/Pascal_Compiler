#pragma once
#include "Expression.h"

class ExprUnOp : public Expression
{
public:
	int operation;
	ExprUnOp(Expression* left, int t);
	~ExprUnOp();
};

