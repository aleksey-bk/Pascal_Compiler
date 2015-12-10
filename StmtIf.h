#pragma once
#include "Statement.h"
#include "StmtBlock.h"

class StmtIf : public Statement
{
public:
	Expression* if_expr;
	Statement* if_block;
	Statement* else_block;
	StmtIf()
	{
		if_block = NULL;
		else_block = NULL;
		if_expr = NULL;
		class_id = CLASS_IF;
	}
	~StmtIf(){};
};

