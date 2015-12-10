#pragma once
#include "SymTypeScalar.h"
class SymTypeFloat : public SymTypeScalar
{
public:
	SymTypeFloat() : SymTypeScalar()
	{
		name = "double";
		sc_id = type_float;
	}
	~SymTypeFloat(){};
};

