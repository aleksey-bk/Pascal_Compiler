#pragma once
#include "Symbol.h"
#include "ValStruct.h"
#include "SymType.h"

enum { var_const, var_global, var_local, var_param };

class SymVar : public Symbol
{
public:
	SymType* var_type;
	ValStruct* val;
	int var_idx;
	SymVar()
	{
		val = NULL;
		class_type = sym_var;
	}
	~SymVar(){};
};

