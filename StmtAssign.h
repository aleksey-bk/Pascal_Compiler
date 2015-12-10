#pragma once
#include "Statement.h"
class StmtAssign : public Statement
{
public:
	Expression* assign_expr;
	StmtAssign()
	{
		assign_expr = NULL;
		class_id = CLASS_ASSIGN;
	}
	~StmtAssign(){};
};

