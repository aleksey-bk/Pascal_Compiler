#pragma once
#include "SymVar.h"
class SymVarConst : public SymVar
{
public:
	SymVarConst() : SymVar()
	{
		var_idx = var_const;
	}
	~SymVarConst(){};
};

