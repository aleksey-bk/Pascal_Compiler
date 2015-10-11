#pragma once
#include "Expression.h"
class ExprRealConst : public Expression
{
public:
	ExprRealConst(double n);
	~ExprRealConst();
};

