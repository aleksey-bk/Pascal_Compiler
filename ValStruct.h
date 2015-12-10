#pragma once
#include "Expression.h"
class ValStruct
{
public:
	SymType* t;
	Expression* val;
	vector<ValStruct*>* arr_val;
	ValStruct()
	{
		t = NULL;
		val = NULL;
		arr_val = NULL;
	};
	~ValStruct(){};
};

