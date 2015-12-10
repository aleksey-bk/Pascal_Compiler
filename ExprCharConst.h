#pragma once
#include "Expression.h"
class ExprCharConst : public Expression
{
public:
	char char_const;
	ExprCharConst(char c, SymType* t);
	~ExprCharConst();
};

