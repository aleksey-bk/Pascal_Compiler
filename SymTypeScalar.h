#pragma once
#include "SymType.h"

// type_integer, type_char, type_float 

enum {type_integer, type_char, type_float};

class SymTypeScalar : public SymType
{
public:
	int sc_id;
	SymTypeScalar() : SymType()
	{
		type_id = type_scalar;
	}
	~SymTypeScalar(){};
};

