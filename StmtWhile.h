#pragma once
#include "Statement.h"
#include "StmtBlock.h"

class StmtWhile : public Statement
{
public:
	Expression* while_expr;
	Statement* while_block;
	StmtWhile()
	{
		while_expr = NULL;
		while_block = NULL;
		class_id = CLASS_WHILE;
	}
	~StmtWhile(){};
};

