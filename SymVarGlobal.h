#pragma once
#include "SymVar.h"
class SymVarGlobal : public SymVar
{
public:
	SymVarGlobal() : SymVar()
	{
		var_idx = var_global;
	}
	~SymVarGlobal(){};
};

