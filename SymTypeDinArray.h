#pragma once
#include "SymTypeArray.h"
class SymTypeDinArray : public SymType
{
public:
	SymType* el_type;
	SymTypeDinArray() : SymType()
	{
		type_id = type_din_array;
	}
	~SymTypeDinArray(){};
};

