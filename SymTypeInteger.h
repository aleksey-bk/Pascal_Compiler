#pragma once
#include "SymTypeScalar.h"
class SymTypeInteger : public SymTypeScalar
{
public:
	SymTypeInteger() : SymTypeScalar()
	{
		name = "integer";
		sc_id = type_integer;
	}
	~SymTypeInteger(){};
};

