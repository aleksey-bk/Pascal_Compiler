#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"



#define IsSimpleOp ((((priority == cmp_inP && S.lexid != KWin) || priority == plus_orP || priority == mul_mod_andP)))
#define ParseF (MaxPriority + 1)

const int Parser::priority_op[MaxPriority + 1][MaxNumOperatorsInP] =
{ { assign }, { eq, not_eq, big, less_, big_or_eq, less_or_eq, KWin }, 
{ Plus, Minus, XOR, OR }, { Mul, Div, DIV, MOD, AND }, { open_br, open_qbr, rec } };
const int Parser::priority_length[MaxPriority + 1] = { 1, 7, 4, 5, 3 };

void Parser::ParserExprTEST()
{
	root = ParseExpr(0, NULL);
	if (!T.End())
		throw GetErrInformation(SaveLex, "bad lex");
	return;
}

void Parser::CheckSign(Lexeme S)
{
	switch (S.type)
	{
	case TypeErr:
	{
		throw GetErrInformation(S, "");
	}
	case TypeOp:
	{
		break;
	}
	case TypeSep:
	{
		if (S.lexid == dot_com)
			break;
		if ((S.lexid == com) || (S.lexid == dot_dot))
			break;
		throw GetErrInformation(S, "unexpected lex");
	}
	default:
	{
		throw GetErrInformation(S, "bad lex");
	}
	}
	return;
}


void Parser::CheckOperand(Lexeme S)
{
		switch (S.type)
		{
		case TypeInteger:
		{
			break;
		}
		case TypeReal:
		{
			break;
		}
		case TypeIdent:
		{
			break;
		}
		case TypeChar:
		{
			break;
		}
		case TypeErr:
		{
			throw GetErrInformation(S, "");
		}
		case TypeEOF:
		{
			throw GetErrInformation(S, "");
		}
		default:
		{
			throw GetErrInformation(S, "unexpected lex");
		}
	}
	return;
}


bool Parser::IsTypename(string s)
{
	if (Table.SearchType(s) != NULL)
		return true;
	return false;
}

Expression* Parser::ParseAssign(Expression* l)
{
	switch (l->type)
	{
	case BinOp:
	{
		if (((ExprBinOp*)l)->operation != open_qbr && ((ExprBinOp*)l)->operation != rec)
			throw GetErrInformation(LastLex, "const assign");
		break;
	}
	case UnOp:
	{
		if (((ExprUnOp*)l)->operation != Cap)
			throw GetErrInformation(LastLex, "const assign");
		break;
	}
	case Var:
	{
		if (Table.SearchVar(((ExprVar*)l)->name)->var_idx != var_const)
			break;
		throw GetErrInformation(LastLex, "const assign");
	}
	default:
		throw GetErrInformation(LastLex, "const assign");
	}
	Expression* right = ParseExpr(1, NULL);
	if (!CheckTypeComp(l->expr_type, right->expr_type))
		throw GetErrInformation(LastLex, "incompatible types");
	return new ExprBinOp(l, assign, right);
}

Expression* Parser::ParseRecord(Expression* l)
{
	if (l->type != Var)
	{
		if (l->type == BinOp)
		{
			switch (((ExprBinOp*)l)->operation)
			{
			case Func:
				break;
			case Cast:
				break;
			case open_qbr:
				break;
			case rec:
				break;
			default:
				throw GetErrInformation(LastLex, "bad record name");
			}
		}
		else
			throw GetErrInformation(LastLex, "bad record name");
	}
	Lexeme field = NextLex();
	if (field.type == TypeIdent)
	{
		ExprVar* r = new ExprVar(field.text, NULL);
		r->expr_type = ((SymTypeRecord*)(l->expr_type))->GetFieldType(field.text);
		if (r->expr_type == NULL)
			throw GetErrInformation(field, "bad field name");
		ExprBinOp* record = new ExprBinOp(l, rec, r);
		record->expr_type = r->expr_type;
		return record;
	}
	throw GetErrInformation(field, "bad right operand"); //?
}

Expression* Parser::ParseFunction(Expression* l)
{
	if (l->type != Var)
		throw GetErrInformation(LastLex, "bad function name");
	if (IsTypename(((ExprVar*)l)->name))
		return ParseCast(l);
	ExprArgs* args = new ExprArgs();
	vector<Expression*> args_vector;
	Expression* E;
	bool args_1_or_more = false;
	while (true)
	{
		E = ParseExpr(1, false);
		if (SaveLex.lexid == com)
		{
			args_vector.push_back(E);
			SaveLex.lexid = -1;
		}
		else
		{
			if (SaveLex.lexid == close_br)
			{
				if (E->type == Args)       //void argument
				{
					if (args_1_or_more)
						throw GetErrInformation(LastLex, "bad function argument");
					args = (ExprArgs*)E;
					SaveLex.lexid = -1;
					return args;
				}
				else
				{
					args_vector.push_back(E);
					break;
				}
			}
			else
				throw GetErrInformation(SaveLex, "bad lex");
		}
		args_1_or_more = true;
	}
	SaveLex.lexid = -1;
	args->args = args_vector;
	ExprBinOp* function_node = new ExprBinOp(l, Func, args);
	//TODO: expr_type for func_node
	return function_node;
}

Expression* Parser::ParseArray(Expression* l)
{
	if (l->type != Var)
	{
		if (l->type == BinOp)
		{
			switch (((ExprBinOp*)l)->operation)
			{
			case Func:
				break;
			case Cast:
				break;
			case open_qbr:
				break;
			case rec:
				break;
			default:
				GetErrInformation(LastLex, "bad array name");
			}
		}
		else
			throw GetErrInformation(LastLex, "bad array name");
	}
	ExprArgs* args = new ExprArgs();
	Expression* E;
	while (true)
	{
		E = ParseExpr(1, NULL);
		if (E->expr_type->type_id != type_scalar)
			throw GetErrInformation(LastLex, "bad array index");
		if (((SymTypeInteger*) (E->expr_type))->sc_id != type_integer)
			throw GetErrInformation(LastLex, "bad array index");
		if (SaveLex.lexid == com)
			SaveLex.lexid = -1;
		else if (SaveLex.lexid == close_qbr)
		{
			if (E->type == Args)
				throw GetErrInformation(LastLex, "bad array index");
			if (!CheckTypeEq(E->expr_type, Table.SearchType("integer")))
				throw GetErrInformation(LastLex, "array index must be integer");
			args->args.push_back(E);
			break;
		}
		else
			throw GetErrInformation(SaveLex, "bad lex");
		args->args.push_back(E);
	}
	SaveLex.lexid = -1;
	ExprBinOp* arr_node = new ExprBinOp(l, open_qbr, args);
	arr_node->expr_type = ((SymTypeArray*)(Table.SearchVar(((ExprVar*)l)->name)->var_type))->el_type;
	return arr_node;
}

Expression* Parser::ParseCast(Expression* l)
{
	Expression* r = ParseExpr(1, false);
	if (SaveLex.lexid != close_br)
		throw GetErrInformation(SaveLex, "bad lex");
	SaveLex.lexid = -1;
	ExprBinOp* cast_node = new ExprBinOp(l, Cast, r);
	cast_node->expr_type = Table.SearchType(((ExprVar*)l)->name);
	return cast_node;
}

Expression* Parser::ParseBinOp(Expression* l, int op_idx, int priority)
{
	Expression* r = ParseExpr(priority + 1, NULL);
	if (!CheckTypeComp(l->expr_type, r->expr_type))
		throw GetErrInformation(LastLex, "incompatible types");
	ExprBinOp* bin_op_node = new ExprBinOp(l, op_idx, r);
	switch (op_idx) //TODO: logic operations
	{
	case Div:
	{
		bin_op_node->expr_type = Table.SearchType("double");
		break;
	}
	case Mul:
	{
		if (CheckTypeEq(bin_op_node->left->expr_type, Table.SearchType("double")) ||
			CheckTypeEq(bin_op_node->right->expr_type, Table.SearchType("double")))
			bin_op_node->expr_type = Table.SearchType("double");
		else
			bin_op_node->expr_type = l->expr_type;
		break;
	}
	case Plus:
	{
		if (CheckTypeEq(bin_op_node->left->expr_type, Table.SearchType("double")) ||
			CheckTypeEq(bin_op_node->right->expr_type, Table.SearchType("double")))
			bin_op_node->expr_type = Table.SearchType("double");
		else
			bin_op_node->expr_type = Table.SearchType("integer");
		break;
	}
	case Minus:
	{
		if (CheckTypeEq(bin_op_node->left->expr_type, Table.SearchType("double")) ||
			CheckTypeEq(bin_op_node->right->expr_type, Table.SearchType("double")))
			bin_op_node->expr_type = Table.SearchType("double");
		else
			bin_op_node->expr_type = Table.SearchType("integer");
		break;
	}
	default:
	{
		bin_op_node->expr_type = Table.SearchType("integer");
		break;
	}
	}
	return bin_op_node;
}

Expression* Parser::ParseDpz()
{
	Expression* left = ParseExpr(2, NULL);
	if (SaveLex.lexid != dot_dot)
		throw GetErrInformation(SaveLex, "expected \"..\"");
	SaveLex.lexid = -1;
	Expression* right = ParseExpr(2, NULL);
	if (SaveLex.lexid != close_qbr)
		throw GetErrInformation(SaveLex, "expected \"]\"");
	if (!CheckTypeEq(left->expr_type, right->expr_type))
		throw GetErrInformation(SaveLex, "types in range not eq");
	SaveLex.lexid = -1;
	ExprBinOp* ret = new ExprBinOp(left, dot_dot, right);
	ret->expr_type = left->expr_type;
	return ret;
}

Expression* Parser::ParseIn(Expression* left)
{
	Expression* right = ParseDpz();
	ExprBinOp* in_node = new ExprBinOp(left, KWin, right);
	in_node->expr_type = left->expr_type;
	return in_node;
}

bool Parser::CheckTypeEq(SymType* l, SymType* r)
{
	if (l == r)
		return true;
	if (l->type_id == type_alias)
		return CheckTypeEq(((SymTypeAlias*)l)->GetFirstType(), r);
	if (r->type_id == type_alias)
		return CheckTypeEq(l, ((SymTypeAlias*)r)->GetFirstType());
	switch (l->type_id)
	{
	case type_scalar:
	{
		if (r->type_id == type_scalar)
		{
			SymTypeScalar* L = (SymTypeScalar*)l;
			SymTypeScalar* R = (SymTypeScalar*)r;
			if (L->sc_id == R->sc_id)
				return true;
		}
		return false;
	}
	case type_array:
	{
		if (r->type_id == type_alias)
			return CheckTypeComp(l, ((SymTypeAlias*)r)->GetFirstType());
		if (r->type_id != type_array)
			throw GetErrInformation(LastLex, "incompatible types");
		SymTypeArray* R = (SymTypeArray*)r;
		SymTypeArray* L = (SymTypeArray*)l;
		if ((int)Calc(L->bounds->left) != (int)Calc(R->bounds->left))
			return false;
		if ((int)Calc(L->bounds->right) != (int)Calc(R->bounds->right))
			return false;
		if (!CheckTypeEq(L->el_type, R->el_type))
			return false;
		return true;
	}
	}
	return false;
}

bool Parser::CheckTypeComp(SymType* l, SymType* r)
{
	if (CheckTypeEq(l, r))
		return true;
	if (l->type_id == type_alias)
		return CheckTypeComp(((SymTypeAlias*)l)->GetFirstType(), r);
	if (r->type_id == type_alias)
		return CheckTypeComp(l, ((SymTypeAlias*)r)->GetFirstType());
	switch (l->type_id) //TODO: add other types
	{
	case type_scalar:
	{
		switch (((SymTypeScalar*)l)->sc_id)
		{
		case type_integer:
		{
			switch (r->type_id)
			{
			case type_scalar:
			{
				if (((SymTypeScalar*)r)->sc_id == type_float)
					return true;
			}
			default:
			{
				return false;
			}
			}
		}
		case type_float:
		{
			switch (r->type_id)
			{
			case type_scalar:
			{
				if (((SymTypeScalar*)r)->sc_id == type_integer)
					return true;
				return false;
			}
			default:
			{
				return false;
			}
			}
		}
		case type_char:
		{
			return false;
		}
		}
	}
	case type_array:
	{
		if (r->type_id != type_array)
			throw GetErrInformation(LastLex, "incompatible types");
		SymTypeArray* R = (SymTypeArray*)r;
		SymTypeArray* L = (SymTypeArray*)l;
		if ((int)Calc(L->bounds->left) != (int)Calc(R->bounds->left))
			throw GetErrInformation(LastLex, "incompatible types");
		if ((int)Calc(L->bounds->right) != (int)Calc(R->bounds->right))
			throw GetErrInformation(LastLex, "incompatible types");
		if (!CheckTypeEq(L->el_type, R->el_type))
			throw GetErrInformation(LastLex, "incompatible types");
		return true;
	}
	}
	return false;
}


Expression* Parser::ParseExpr(int priority, Expression* L)
{
	//TODO: assign only 1 time in expression
	Expression* left;
	if (!L)
	{
		if (priority == ParseF) 
			return ParseFactor();
		left = ParseExpr(priority + 1, NULL);
	}
	else
		left = L;
	Lexeme S;
	if (SaveLex.lexid == -1)
	{
		S = NextLex();
		if (S.type == TypeEOF)
			return left;
		if (S.lexid == cap)
		{
			if (left->type != Var)
				throw GetErrInformation(S, "argument must be var");
			if (left->expr_type->type_id != type_pointer)
				throw GetErrInformation(S, "invalid operation");
			Expression* new_left = new ExprUnOp(left, Cap);
			new_left->expr_type = ((SymTypePointer*)(left->expr_type))->p_type;
			return ParseExpr(priority, new_left);
		}
	}
	else
	{
		S = SaveLex;
		SaveLex.lexid = -1;
	}
	int op_idx;
	Expression* new_left = NULL;
	for (int i = 0; i < priority_length[priority]; ++i)
	{
		if (S.lexid == priority_op[priority][i])
		{
			op_idx = priority_op[priority][i];
			if (IsSimpleOp)
				new_left = ParseBinOp(left, op_idx, priority);
			else
			{
				switch (op_idx)
				{
				case open_qbr:
				{
					new_left = ParseArray(left);
					break;
				}
				case rec:
				{
					new_left = ParseRecord(left);
					break;
				}
				case assign:
				{
					new_left = ParseAssign(left);
					break;
				}
				case KWin:
				{
					Lexeme next = NextLex(); //TODO: move to ParseDpz
					if (next.lexid != open_qbr) //__
						throw GetErrInformation(next, "expected \"[\""); //__
					new_left = ParseIn(left);
					break;
				}
				case open_br:
				{
					new_left = ParseFunction(left);
					break;
				}
				}
			}
			if (T.End())
				return new_left;
			return ParseExpr(priority, new_left);
		}
	}
	SaveLex = S;
	return left;
}


Expression* Parser::ParseFactor()
{
	Lexeme LL = LastLex;
	Lexeme lex = NextLex();
	if (LL.lexid == Minus && lex.lexid == Minus)
		throw GetErrInformation(lex, "bad minus");
	if (lex.lexid == close_br)
	{
		ExprArgs* void_arg = new ExprArgs();
		void_arg->void_arg = true;
		void_arg->type = Args;
		SaveLex = lex;
		return void_arg;
	}
	if (lex.lexid == open_br)
	{
		Expression* r = ParseExpr(1, NULL);
		if (SaveLex.lexid == close_br)
		{
			SaveLex.lexid = -1;
			return r;
		}
		else
			throw GetErrInformation(SaveLex, "expected \")\"");
	}
	int t;
	if ((t = IsUnaryOp(lex)) != -1)
	{
		ExprUnOp* m = new ExprUnOp(ParseExpr(MaxPriority, NULL), t); // ??? MaxPriority but its work
		if (lex.lexid == dog)
		{
			SymTypePointer* p = new SymTypePointer;
			m->expr_type = p;
			p->p_type = m->left->expr_type;
		}
		else
			m->expr_type = m->left->expr_type;
		return m;
	}
	CheckOperand(lex);
	if (lex.type == TypeInteger)
		return new ExprIntConst(stoi(lex.text), (SymType*)Table.SearchType("integer"));
	if (lex.type == TypeReal)
		return new ExprRealConst(stod(lex.text), (SymType*)Table.SearchType("double"));
	if (lex.type == TypeChar)
		return new ExprCharConst(lex.text[1], (SymType*)Table.SearchType("char"));
	if (lex.type == TypeIdent)
	{
		if (IsTypename(lex.text))
		{
			ExprVar* v = new ExprVar(lex.text, NULL);
			return v;
		}
		//TODO: if (IsFuncName()... 
		SymVar* var = Table.SearchVar(lex.text);
		if (var == NULL)
			throw GetErrInformation(LastLex, "undefined symbol");
		ExprVar* v = new ExprVar(lex.text, var->var_type);
		return v;
	}
	return NULL;
}

int Parser::IsUnaryOp(Lexeme l)   //TODO: add new operations
{
	switch (l.lexid)
	{
	case Minus:
		return UnMinus;
	case dog:
		return Dog;
	case NOT:
		return Not;
	case cap:
		return Cap;
	}
	return -1;          
}


void Parser::PrintNode(Expression* node, int h, string* ws)
{
	int reindex_length = 22;
	static int reindex_array[] = {Func, Cast, open_qbr, assign, rec, Plus, 
	Minus, Mul, Div, DIV, MOD, OR, XOR, AND, eq, not_eq, big, big_or_eq, less_, less_or_eq, KWin, dot_dot};
	static string print_array[] = { "F", "T", "[ ]", ":=", ".", "+",
	"-", "*", "/", "DIV", "MOD", "OR", "XOR", "AND", "=", "<>", ">", ">=", "<", "<=", "IN", ".." };
	static int first_arg_h = -1;
	if (node->right)	
		PrintNode(node->right, h + 1, ws);
	if (first_arg_h == -1)
	{
		for (int i = 0; i < h * 4; ++i)
			*ws += " ";
	}
	else
	{
		for (int i = 0; i < (h - first_arg_h) * 4; ++i)
			*ws += " ";
		first_arg_h = -1;
	}
	int a = node->type;
	bool _break = false;
	switch (node->type)
	{
	case Var:
	{
		*ws += ((ExprVar*)node)->name.c_str();
		break;
	}
	case IntConst:
	{
		*ws += to_string(((ExprIntConst*)node)->int_const);
		break;
	}
	case RealConst:
	{
		*ws += to_string(((ExprRealConst*)node)->real_const);
		break;
	}
	case BinOp:
	{
		int op = ((ExprBinOp*)node)->operation;
		for (int i = 0; i < reindex_length; ++i)
		{
			if (reindex_array[i] == op)
			{
				*ws += print_array[i];
				break;
			}
		}
		break;
	}
	case Args:
	{
		if (((ExprArgs*)node)->void_arg)
		{
			_break = true;
			*ws += "***\n\n";
			break;
		}
		ExprArgs* n = ((ExprArgs*)node);
		int l = n->args.size();
		for (int i = 0; i < l; ++i)
		{
			if (i == 0)
			{
				first_arg_h = h;
				PrintNode(n->args.at(i), h, ws);
			}
			else
				PrintNode(n->args.at(i), h, ws);
		}
		break;
	}
	case UnOp:
	{
		if (_break)
			break;
		switch (((ExprUnOp*)node)->operation)
		{
		case UnMinus:
		{
			_break = true;
			*ws += "-";
			break;
		}
		case NOT:
		{
			_break = true;
			*ws += "NOT";
			break;
		}
		case dog:
		{
			_break = true;
			*ws += "@";
			break;
		}
		}
	}
	}
	if (node->type != Args)
		*ws += "\n\n";
	if (node->left)
		PrintNode(node->left, h + 1, ws);
	return;
}

void Parser::PrintExprTree(string FileName, bool clean_file)
{
	if (root == NULL)
		return;
	if (clean_file)
	{
		ofstream s(FileName, ios::trunc);
		s.close();
	}
	string* ws = new string;
	PrintNode(root, 0, ws);
	ofstream s(FileName, ios::app);
	s << (*ws);
	delete ws;
	return;
}

void NeedBrackets(Expression* e, string* ws)
{
	switch (e->type)
	{
	case Var:
		break;
	case IntConst:
		break;
	case RealConst:
		break;
	case CharConst:
		break;
	case BinOp:
	{
		ExprBinOp* b = (ExprBinOp*)e;
		if (b->operation == Plus || b->operation == rec || b->operation == open_qbr)
			break;
		*ws = "(" + *ws + ")";
		break;
	}
	default:
	{
		*ws = "(" + *ws + ")";
	}
	}
	return;
}

string* Parser::PrintExprPart(Expression* expr)
{
	string* ws = new string;
	switch (expr->type)
	{
	case BinOp:
	{
		ExprBinOp* e = (ExprBinOp*)expr;
		switch (e->operation)
		{
		case Plus:
		{
			string* l = PrintExprPart(expr->left);
			string* r = PrintExprPart(expr->right);
			*ws += *l + " + " + *r;
			break;
		}
		case Minus:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " - " + *r;
			break;
		}
		case Mul:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " * " + *r;
			break;
		}
		case Div:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " / " + *r;
			break;
		}
		case rec:
		{
			string* l = PrintExprPart(expr->left);
			string* r = PrintExprPart(expr->right);
			*ws += *l + "." + *r;
			break;
		}
		case open_qbr:
		{
			string* l = PrintExprPart(expr->left);
			string* r = PrintExprPart(expr->right);
			*ws += *l + "[" + *r + "]";
			break;
		}
		case open_br:
		{
			string* l = PrintExprPart(expr->left);
			string* r = PrintExprPart(expr->right);
			*ws += *l + "(" + *r + ")";
			break;
		}
		case big:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " > " + *r;
			break;
		}
		case big_or_eq:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " >= " + *r;
			break;
		}
		case less_:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " < " + *r;
			break;
		}
		case less_or_eq:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " <= " + *r;
			break;
		}
		case eq:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " = " + *r;
			break;
		}
		case assign:
		{
			string* l = PrintExprPart(expr->left);
			string* r = PrintExprPart(expr->right);
			*ws += *l + " := " + *r;
			break;
		}
		case AND:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " and " + *r;
			break;
		}
		case OR:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " or " + *r;
			break;
		}
		case XOR:
		{
			string* l = PrintExprPart(expr->left);
			NeedBrackets(expr->left, l);
			string* r = PrintExprPart(expr->right);
			NeedBrackets(expr->right, r);
			*ws += *l + " xor " + *r;
			break;
		}
		}
		break;
	}
	case UnOp:
	{
		ExprUnOp* e = (ExprUnOp*)expr;
		switch (e->operation)
		{
		case UnMinus:
		{
			*ws += "-";
			string* l = PrintExprPart(e->left);
			NeedBrackets(e->left, l);
			*ws += *l;
			delete l;
			break;
		}
		case Cap:
		{
			string* l = PrintExprPart(e->left);
			NeedBrackets(e->left, l);
			*ws += *l;
			*ws += "^";
			delete l;
			break;
		}
		case Not:
		{
			*ws += "not ";
			string* l = PrintExprPart(e->left);
			NeedBrackets(e->left, l);
			*ws += *l;
			delete l;
			break;
		}
		case Dog:
		{
			*ws += "@";
			string* l = PrintExprPart(e->left);
			NeedBrackets(e->left, l);
			*ws += *l;
			delete l;
			break;
		}
		}
		break;
	}
	case Args:
	{
		ExprArgs* e = (ExprArgs*)expr;
		int l = e->args.size();
		for (int i = 0; i < l; ++i)
		{
			*ws += "(";
			string* arg = PrintExprPart(e->args[i]);
			*ws += *arg;
			if (i == l - 1)
				*ws += ")";
			else
				*ws += ", ";
			delete arg;
		}
		break;
	}
	case IntConst:
	{
		*ws += to_string(((ExprIntConst*)expr)->int_const);
		break;
	}
	case RealConst:
	{
		string d_to_s = to_string(((ExprRealConst*)expr)->real_const);
		int i = 0;
		while (true)
		{
			if (d_to_s[i] == '.')
			{
				d_to_s = d_to_s.substr(0, i + 3);
				break;
			}
		}
		*ws += d_to_s;
		break;
	}
	case CharConst:
	{
		*ws += "'";
		ws->push_back(((ExprCharConst*)expr)->char_const);
		*ws += "'";
		break;
	}
	case Var:
	{
		*ws += ((ExprVar*)expr)->name;
		break;
	}
	}
	return ws;
}

void Parser::PrintExpr(string FileName, Expression* expr, bool clean_file)
{
	if (clean_file)
	{
		ofstream s(FileName, ios::trunc);
		s.close();
	}
	string* ws = PrintExprPart(expr);
	ofstream s(FileName, ios::app);
	s << (*ws);
	delete ws;
	return;
}

void Parser::PrintExprMain(string FileName)
{
	PrintExpr(FileName, root, true);
	return;
}

Parser::~Parser()
{
	delete root;
}
