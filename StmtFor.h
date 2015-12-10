#pragma once
#include "Statement.h"
#include "StmtBlock.h"

class StmtFor : public Statement
{
public:
	ExprVar* v;
	Expression* expr_1;
	Expression* expr_2;
	Statement* for_block;
	bool to;
	StmtFor(bool arg_to)
	{
		to = arg_to;
		v = NULL;
		for_block = NULL;
		expr_1 = NULL;
		expr_2 = NULL;
		class_id = CLASS_FOR;
	}
	~StmtFor(){};
};

