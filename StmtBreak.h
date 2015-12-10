#pragma once
#include "Statement.h"
class StmtBreak : public Statement
{
public:
	StmtBreak()
	{
		class_id = CLASS_BREAK;
	}
	~StmtBreak();
};

