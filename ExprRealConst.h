#pragma once
#include "Expression.h"
class ExprRealConst : public Expression
{
public:
	double real_const;
	ExprRealConst(double n);
	~ExprRealConst();
};

