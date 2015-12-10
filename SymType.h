#pragma once
#include "Symbol.h"

enum { type_scalar, type_array, type_din_array, type_pointer, type_record, type_alias };

class SymType : public Symbol
{
public:
	int type_id;
	SymType()
	{
		class_type = sym_type;
	}
	~SymType(){};
};

