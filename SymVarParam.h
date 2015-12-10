#pragma once
#include "SymVar.h"
class SymVarParam :
	public SymVar
{
public:
	SymVarParam() : SymVar()
	{
		var_idx = var_param;
	}
	~SymVarParam(){};
};

