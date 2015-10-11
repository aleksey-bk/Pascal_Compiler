#pragma once
#include "Expression.h"
class ExprIntConst : public Expression
{
public:
	ExprIntConst(int n);
	~ExprIntConst();
};

