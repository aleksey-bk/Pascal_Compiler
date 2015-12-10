#include "Symtable.h"


Symtable::Symtable()
{
	AddSymType(new SymTypeInteger());
	AddSymType(new SymTypeChar());
	AddSymType(new SymTypeFloat());
}


Symtable::~Symtable()
{

}

SymProc* Symtable::SearchProc(string name) //TODO:
{
	return NULL;
}


void Symtable::AddSymVar(SymVar* a)
{
	table_vars.push_back(a);
	return;
}

void Symtable::AddSymType(SymType* a)
{
	table_types.push_back(a);
	return;
}


SymType* Symtable::SearchType(string name)
{
	int l = table_types.size();
	for (int i = 0; i < l; ++i)
	{
		if (table_types[i]->name == name)
			return table_types[i];
	}
	return NULL;
}

double Calc(Expression* e)
{
	switch (e->type)
	{
	case BinOp:
	{
		switch (((ExprBinOp*)e)->operation)
		{
		case Plus:
		{
			return Calc(e->left) + Calc(e->right);
		}
		case Minus:
		{
			return Calc(e->left) - Calc(e->right);
		}
		case Mul:
		{
			return Calc(e->left) * Calc(e->right);
		}
		case Div:
		{
			return Calc(e->left) / Calc(e->right);
		}
		}
	}
	case IntConst:
		return (((ExprIntConst*)e)->int_const);
	case RealConst:
		return (((ExprRealConst*)e)->real_const);
	}
}

string ExprToChar(Expression* ec)
{
	return string(string("'") + ((ExprCharConst*)ec)->char_const + string("'"));
}


SymVar* Symtable::SearchVar(string name)
{
	int l = table_vars.size();
	for (int i = 0; i < l; ++i)
	{
		if (table_vars[i]->name == name)
			return table_vars[i];
	}
	return NULL;
}

void Symtable::PrintVal(string* ws, ValStruct* v, bool arr_el)
{
	switch (v->t->type_id)
	{
	case type_scalar:
	{
		switch (((SymTypeScalar*)v->t)->sc_id)
		{
		case type_integer:
		{
			if (!arr_el)
				*ws += " = ";
			*ws += (to_string((int)Calc(v->val)));
			return;
		}
		case type_float:
		{
			if (!arr_el)
				*ws += " = ";
			string double_in_string;
			double_in_string = to_string(Calc(v->val));
			int l = double_in_string.length();
			for (int i = 0; i < l; ++i)  //2 signs after dot
			{
				if (double_in_string[i] == '.')
				{
					double_in_string = double_in_string.substr(0, i + 3);
					*ws += double_in_string;
					return;
				}
			}
		}
		case type_char:
		{
			if (!arr_el)
				*ws += " = ";
			*ws += ExprToChar(v->val);
			return;
		}
		}
	}
	case type_array:
	{
		int l = v->arr_val->size();
		if (!arr_el)
			*ws += " = ";
		*ws += "(";
		for (int i = 0; i < l; ++i)
		{
			PrintVal(ws, (*(v->arr_val))[i], true);
			if (i < l - 1)
				*ws += ", ";
		}
		*ws += ")";
		return;
	}
	case type_alias:
	{
		ValStruct* v2 = new ValStruct(*v);
		v2->t = ((SymTypeAlias*)v->t)->GetFirstType();
		PrintVal(ws, v2, arr_el);
		delete v2; // TODO: destructor for this class
		return;
	}
	}
	return;
}


string Symtable::RecToCode(SymType* t)
{
	SymTypeRecord* r = (SymTypeRecord*)t;
	string ws = "record\n";
	bool one_type = false;
	int l = r->fields.size();
	for (int i = 0; i < l; ++i)
	{
		SymVar* v = r->fields[i];
		if (one_type)
			ws += r->fields[i]->name;
		else
			ws += "    " + r->fields[i]->name;
		if (i < l - 1)
		{
			if ((r->fields[i + 1])->var_type == v->var_type)   //may be bad eq
			{
				ws += ", ";
				one_type = true;
			}
			else
			{
				ws += ": " + TypeToCode(v->var_type);
				one_type = false;
			}
		}
		else
		{
			ws += ": " + TypeToCode(v->var_type);
			one_type = false;
		}
		if (!one_type)
			ws += ";\n";
	}
	ws += "  end";
	return ws;
}


string* Symtable::GetPrintTypes()
{
	int l = table_types.size();
	if (l == 3)
		return new string("");
	string* ws = new string("type\n");
	for (int i = 3; i < l; ++i)
	{
		*ws += "  " + table_types[i]->name + " = ";
		if (table_types[i]->type_id == type_alias)
			*ws += TypeToCode((SymType*)((SymTypeAlias*)table_types[i])->alias_type) + ";\n";
		if (table_types[i]->type_id == type_record)
			*ws += RecToCode(table_types[i]) + ";\n";
	}
	return ws;
}

string* Symtable::GetPrintConst()
{
	int l = table_vars.size();
	if (l == 0)
		return new string();
	for (int i = 0; i < l; ++i)
	{
		if (((SymVar*)table_vars[i])->var_idx == var_const)
			break;
		if (i == l - 1)
			return new string("");
	}
	string* ws = new string("const\n");
	for (int i = 0; i < l; ++i)
	{
		SymVar* v = (SymVar*)table_vars[i];
		if (v->var_idx != var_const)
			continue;
		*ws += "  " + table_vars[i]->name + ": " +//_________________________________________
			TypeToCode(v->var_type);
		if (v->val != NULL)
			PrintVal(ws, ((SymVar*)table_vars[i])->val, false);
		*ws += ";\n";
	}
	return ws;
}

string* Symtable::GetPrintVars()
{
	bool one_type = false;
	int l = table_vars.size();
	if (l == 0)
		return new string();
	for (int i = 0; i < l; ++i)
	{
		SymVar* v = (SymVar*)table_vars[i];
		if (v->var_idx != var_const)
			break;
		if (i == l - 1)
			return new string("");
	}
	string* ws = new string("var\n");
	for (int i = 0; i < l; ++i)
	{
		SymVar* v = (SymVar*)table_vars[i];
		if (v->var_idx == var_const)
			continue;
		if (one_type)
			*ws += table_vars[i]->name;
		else
			*ws += "  " + table_vars[i]->name;
		if (i < l - 1 && v->val == NULL)
		{
			if (((SymVar*)table_vars[i + 1])->var_idx == var_const || ((SymVar*)table_vars[i + 1])->val != NULL)
			{
				*ws += ": " + TypeToCode(v->var_type);
				one_type = false;
			}
			if (((SymVar*)table_vars[i + 1])->var_type == v->var_type)   //may be bad eq
			{
				*ws += ", ";
				one_type = true;
			}
			else
			{
				*ws += ": " + TypeToCode(v->var_type);
				one_type = false;
			}
		}
		else
		{
			*ws += ": " + TypeToCode(v->var_type);
			one_type = false;
		}
		if (v->val != NULL)
			PrintVal(ws, v->val, false);
		if (!one_type)
			*ws += ";\n";
	}
	return ws;
}


string Symtable::TypeToCode(SymType* t)
{
	switch (t->type_id)   //add new types__________________________________________________
	{
	case type_scalar:
	{
		return t->name;
	}
	case type_alias:
	{
		return t->name;
	}
	case type_din_array:
	{
		return ("array of " + TypeToCode(((SymTypeDinArray*)t)->el_type));
	}
	case type_array:
	{
		return ("array " + string("[") + std::to_string((int)Calc(((SymTypeArray*)t)->bounds->left))
			+ ".." + std::to_string((int)Calc(((SymTypeArray*)t)->bounds->right)) 
			+ string("]") + string(" of ") + TypeToCode(((SymTypeArray*)t)->el_type));
	}
	case type_record:
	{
		return t->name;
	}
	}
}