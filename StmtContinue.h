#pragma once
#include "Statement.h"
class StmtContinue : public Statement
{
public:
	StmtContinue()
	{
		class_id = CLASS_CONTINUE;
	}
	~StmtContinue();
};

