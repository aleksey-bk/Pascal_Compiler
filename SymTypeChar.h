#pragma once
#include "SymTypeScalar.h"
#include "ExprCharConst.h"
class SymTypeChar : public SymTypeScalar
{
public:
	SymTypeChar() : SymTypeScalar()
	{
		name = "char";
		sc_id = type_char;
	}
	~SymTypeChar(){};
};

