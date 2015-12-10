#pragma once
#include "Statement.h"
#include <list>
using namespace std;

class StmtBlock : public Statement
{
public:
	list<Statement*> SList;
	StmtBlock()
	{
		class_id = CLASS_BLOCK;
	}
	~StmtBlock(){};
};

