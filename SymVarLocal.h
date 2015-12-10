#pragma once
#include "SymVar.h"
class SymVarLocal : public SymVar
{
public:
	SymVarLocal() : SymVar()
	{
		var_idx = var_local;
	}
	~SymVarLocal(){};
};

