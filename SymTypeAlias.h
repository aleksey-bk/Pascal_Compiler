#pragma once
#include "SymType.h"
class SymTypeAlias : public SymType
{
public:
	SymType* alias_type;
	SymTypeAlias() : SymType()
	{
		type_id = type_alias;
	}
	~SymTypeAlias(){};
	SymType* GetFirstType()
	{
		SymType* a = alias_type;
		while (true)
		{
			if (a->type_id != type_alias)
				return a;
			else
				a = ((SymTypeAlias*)a)->alias_type;
		}
	}
};

