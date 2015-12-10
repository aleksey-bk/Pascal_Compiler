#pragma once
#include "SymProc.h"
#include "SymType.h"

class SymFunc : public SymProc
{
public:
	SymType* ret_type;
	SymFunc() : SymProc()
	{
		is_func = true;
	}
	~SymFunc(){};
};

