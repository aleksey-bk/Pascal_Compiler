#include "Parser.h"

void Parser::ParserDeclTEST()
{
	ParseDeclaration();
	return;
}


void Parser::ParseSection(string section_name)
{
	if (section_name == "type")
		ParseTypeSection();
	if (section_name == "var")
		ParseVarSection();
	if (section_name == "const")
		ParseConstSection();
}

bool Parser::IsSectionName(Lexeme L)
{
	return (L.text == "type" || L.text == "var" || L.text == "const");
}

double Parser::Calc(Expression* e)
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


int Parser::CalcLength(Expression* e)
{
	return Calc(e->right) - Calc(e->left) + 1; //TODO: bad calc
}


SymType* Parser::ParseType()
{
	// var a: 1..9; b: array [char] of Integer;
	Lexeme next = NextLex();
	if (next.lexid == KWarray)
	{
		next = NextLex();
		if (next.lexid == open_qbr)
		{
			Expression* dpz = ParseDpz();
			if (dpz->expr_type != Table.SearchType("integer"))
				throw GetErrInformation(LastLex, "array index must be integer");
			auto ret = new SymTypeArray;
			ret->bounds = dpz;
			next = NextLex();
			if (next.lexid != KWof)
				throw GetErrInformation(next, "keyword \"of\" expected");
			ret->el_type = ParseType();
			return ret;
		}
		if (next.lexid != KWof)
			throw GetErrInformation(next, "keyword \"of\" expected");
		auto ret = new SymTypeDinArray;
		ret->el_type = ParseType();
		return ret;
	}
	if (next.lexid == cap)
	{
		next = NextLex();
		if (next.lexid == cap)
			throw GetErrInformation(LastLex, "double cap");
		if (next.lexid == KWarray)
			throw GetErrInformation(LastLex, "expected type name, not array");
		SymType* t = Table.SearchType(next.text);
		if (t == NULL)
			throw GetErrInformation(next, "unknown type name");
		SymTypePointer* p = new SymTypePointer;
		p->p_type = t;
		return p;	
	}
	SymType* t = (SymType*)Table.SearchType(next.text);
	if (t == NULL)
		throw GetErrInformation(next, "unknown type name");
	return t;
}



ValStruct* Parser::ParseInit(SymType* t)
{
	Lexeme next;
	if (t->type_id == type_record)
		throw GetErrInformation(next, "record cant be init");
	if (t->type_id == type_array)
	{
		ValStruct* ret = new ValStruct;
		next = NextLex();
		if (next.lexid != open_br)
			throw GetErrInformation(next, "expected \"(\"");
		SymTypeArray* T = (SymTypeArray*)t;
		int l = CalcLength(T->bounds);   //TODO: may be bad calc
		ret->t = t;
		ret->arr_val = new vector<ValStruct*>;
		for (int i = 0; i < l; ++i)
		{
			ret->arr_val->push_back(ParseInit(T->el_type));
			if (T->el_type->type_id == type_array && i < l - 1)
				if (NextLex().lexid != com)
					throw GetErrInformation(LastLex, "unexpected lex, expected \",\"");
		}
		if (T->el_type->type_id == type_array)
		{
			if (NextLex().lexid != close_br)
				throw GetErrInformation(LastLex, "unexpected lex, expected \")\"");
		}
		return ret;
	}
	if (t->type_id == type_scalar)
	{
		ValStruct* ret = new ValStruct;
		ret->val = ParseExpr(2, NULL); //TODO: type check
		ret->t = t;
		if (SaveLex.lexid != dot_com && SaveLex.lexid != com && SaveLex.lexid != close_br)
			throw GetErrInformation(SaveLex, "unexpected lex");
		SaveLex.lexid = -1;
		return ret;
	}
	if (t->type_id == type_alias)
	{
		ValStruct* ret = ParseInit(((SymTypeAlias*)t)->alias_type);
		ret->t = t;
		return ret;
	}
}

void Parser::ParseVarSection()
{
	bool first = true;
	while (true)
	{
		vector<string> var_names;
		SymType* t = NULL;
		ValStruct* Value = NULL;
		Lexeme first_var_name = NextLex();
		if (first_var_name.lexid == KWbegin)
		{ 
			LexBuffer.push(first_var_name);
			return;
		}
		if (IsSectionName(first_var_name))
		{
			if (first)
				throw GetErrInformation(first_var_name, "expected var name");
			else
			{
				ParseSection(first_var_name.text);
				return;
			}
		}
		first = false;
		if (first_var_name.type != TypeIdent)
			GetErrInformation(first_var_name, "bad var name");
		var_names.push_back(first_var_name.text);
		Lexeme next = NextLex();
		if (next.lexid == com)
		{
			while (true)
			{
				next = NextLex();
				if (next.type != TypeIdent)
					GetErrInformation(first_var_name, "bad var name");
				var_names.push_back(next.text);
				next = NextLex();
				if (next.lexid == double_dot)
					break;
				if (next.lexid != com)
					throw GetErrInformation(next, "expected \",\"");
			}
		}
		if (next.lexid != double_dot) //.. : colon
			throw GetErrInformation(next, "expected \":\"");
		t = ParseType();
		next = NextLex();
		if (next.lexid == eq)
		{
			if (var_names.size() > 1)
				throw GetErrInformation(next, "only one variable may be init");
			Value = ParseInit(t);
			if (t->type_id == type_alias)
			{
				if (((SymTypeAlias*)t)->GetFirstType()->type_id == type_array)
				{
					if (NextLex().lexid != dot_com)
						throw GetErrInformation(LastLex, "expected \";\"");
				}
			}
			if (t->type_id == type_array)
			{
				if (NextLex().lexid != dot_com)
					throw GetErrInformation(LastLex, "expected \";\"");
			}
		}
		else
		{
			if (next.lexid != dot_com) // ; semicolon
				throw GetErrInformation(next, "expected \";\"");
		}
		int l = var_names.size();
		for (int i = 0; i < l; ++i)
		{
			SymVarGlobal* v = new SymVarGlobal;  //TODO: only global
			v->var_type = t;
			v->name = var_names[i];
			v->val = Value;
			Table.AddSymVar(v);
		}
	}
	return;
}

void Parser::ParseConstSection()
{
	bool first = true;
	while (true)
	{
		SymVarConst* a = new SymVarConst;
		SymType* t = NULL;
		Lexeme var_name = NextLex();
		if (var_name.lexid == KWbegin)
		{
			LexBuffer.push(var_name);
			return;
		}
		if (IsSectionName(var_name))
		{
			if (first)
				throw GetErrInformation(var_name, "expected var name");
			else
			{
				ParseSection(var_name.text);
				return;
			}
		}
		first = false;
		if (var_name.type != TypeIdent)
			GetErrInformation(var_name, "bad var name");
		a->name = var_name.text;
		Lexeme Next = NextLex();
		if (Next.lexid != eq) //type
		{
			if (Next.lexid == double_dot)
				t = ParseType();
			else
				throw GetErrInformation(Next, "expected \":\"");
			if (t->type_id == type_record)
				throw GetErrInformation(LastLex, "record cant be const");
			Next = NextLex();
			a->var_type = t;
			if (Next.lexid != eq)
			{
				if (Next.lexid == dot_com)
					continue;
				else
					throw GetErrInformation(Next, "expected \"=\"");
			}
			a->val = ParseInit(t);
			if (t->type_id == type_array)
				if (NextLex().lexid != dot_com)
					throw GetErrInformation(LastLex, "expected \";\"");
			if (t->type_id == type_alias)
				if (((SymTypeAlias*)t)->GetFirstType()->type_id == type_array)
					if (NextLex().lexid != dot_com)
						throw GetErrInformation(LastLex, "expected \";\"");
			Table.AddSymVar(a);
			continue;
		}
		else //no type
		{
			if (Next.lexid != eq)
				throw GetErrInformation(Next, "expected \"=\"");
			a->val = new ValStruct;
			a->val->val = ParseExpr(2, NULL);
			a->val->t = a->val->val->expr_type;
			a->var_type = a->val->t;
			Table.AddSymVar(a);
			continue;
		}
	}
	return;
}

void Parser::ParseTypeSection()
{
	bool first = true;
	while (true)
	{
		Lexeme type_name = NextLex();
		if (type_name.lexid == KWbegin)
		{
			LexBuffer.push(type_name);
			return;
		}
		if (IsSectionName(type_name))
		{
			if (first)
				throw GetErrInformation(type_name, "expected type name");
			else
			{
				ParseSection(type_name.text);
				return;
			}
		}
		first = false;
		if (type_name.type != TypeIdent)
			throw GetErrInformation(type_name, "bad type name");
		Lexeme next = NextLex();
		if (next.lexid != eq)
			throw GetErrInformation(next, "expected \"=\"");
		NextLex();
		if (LastLex.lexid == KWrecord)
		{
			ParseRecord(type_name.text);
			continue;
		}
		else
			LexBuffer.push(LastLex);
		SymTypeAlias* t = new SymTypeAlias;
		t->alias_type = ParseType();
		next = NextLex();
		if (next.lexid != dot_com)
			throw GetErrInformation(next, "expected \";\"");
		t->name = type_name.text;
		Table.AddSymType(t);
	}
	return;
}

void Parser::ParseRecord(string rec_name)
{
	SymTypeRecord* ret = new SymTypeRecord;
	ret->name = rec_name;
	while (true)
	{
		int l = 0;
		vector<string> field_names;
		NextLex();
		if (LastLex.lexid == KWend)
		{
			NextLex();
			if (LastLex.lexid != dot_com)
				throw GetErrInformation(LastLex, "expected \";\"");
			Table.AddSymType(ret);
			return;
		}
		if (LastLex.type != TypeIdent)
			throw GetErrInformation(LastLex, "bad field name");
		field_names.push_back(LastLex.text);
		++l;
		while (true)
		{
			NextLex();
			if (LastLex.lexid == com)
			{
				NextLex();
				if (LastLex.type != TypeIdent)
					throw GetErrInformation(LastLex, "bad field name");
				field_names.push_back(LastLex.text);
				++l;
				continue;
			}
			if (LastLex.lexid == double_dot)
				break;
			throw GetErrInformation(LastLex, "unexpected lex");
		}
		SymType* t = ParseType();
		if (NextLex().lexid != dot_com)
			throw GetErrInformation(LastLex, "expected \";\"");
		for (int i = 0; i < l; ++i)
		{
			SymVar* v = new SymVar;
			v->name = field_names[i];
			v->var_type = t;
			ret->fields.push_back(v);
		}
	}
	return;
}


void Parser::ParseDeclaration()
{
	Lexeme L = NextLex();
	if (IsSectionName(L))
	{
		if (L.text == "type")
			ParseTypeSection();
		if (L.text == "const")
			ParseConstSection();
		if (L.text == "var")
			ParseVarSection();
	}
	return;  //____
}

void Parser::PrintDecl(string FileName, bool clean_file)
{
	if (clean_file)
	{
		ofstream s(FileName, ios_base::trunc);
		s.close();
	}
	ofstream out_stream(FileName, ios_base::app);
	string* ws = Table.GetPrintTypes();
	out_stream << ws->c_str();
	delete ws;
	ws = Table.GetPrintConst();
	out_stream << ws->c_str();
	delete ws;
	ws = Table.GetPrintVars();
	out_stream << ws->c_str();
	delete ws;
	out_stream.close();
	return;
}