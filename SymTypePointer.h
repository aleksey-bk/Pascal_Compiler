#pragma once
#include "SymType.h"
class SymTypePointer : public SymType
{
public:
	SymType* p_type;
	SymTypePointer() : SymType()
	{
		type_id = type_pointer;
	}
	~SymTypePointer(){};
};

