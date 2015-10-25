#pragma once
#include "Expression.h"
class ExprIntConst : public Expression
{
public:
	int int_const;
	ExprIntConst(int n);
	~ExprIntConst();
};

