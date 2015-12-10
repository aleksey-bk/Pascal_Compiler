#pragma once
#include "Symbol.h"
class SymProc : public Symbol
{
public:
	bool is_func;
	SymProc()
	{
		class_type = sym_proc;
	}
	~SymProc(){};
};

