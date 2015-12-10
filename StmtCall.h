#pragma once
#include "Statement.h"

class StmtCall : public Statement
{
public:
	ExprBinOp* proc_expr;
	StmtCall()
	{
		proc_expr = NULL;
		class_id = CLASS_CALL;
	}
	~StmtCall(){};
};

