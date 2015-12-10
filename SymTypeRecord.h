#pragma once
#include "SymType.h"
#include "SymVar.h"

class SymTypeRecord : public SymType
{
public:
	vector<SymVar*> fields;
	SymType* GetFieldType(string field_name)
	{
		int l = fields.size();
		for (int i = 0; i < l; ++i)
			if (fields[i]->name == field_name)
				return fields[i]->var_type;
		return NULL;
	}
	SymTypeRecord() : SymType()
	{
		type_id = type_record;
	}
	~SymTypeRecord(){};
};

