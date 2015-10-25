#pragma once
#include "Expression.h"

class ExprArgs : public Expression
{
public:
	bool void_arg;
	vector<Expression*> args;
	ExprArgs();
	~ExprArgs();
};

