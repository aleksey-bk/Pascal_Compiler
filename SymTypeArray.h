#pragma once
#include "SymType.h"
#include "Expression.h"
class SymTypeArray : public SymType
{
public:
	SymType* el_type;
	Expression* bounds;
	SymTypeArray() : SymType()
	{
		type_id = type_array;
	}
	~SymTypeArray(){};
};

