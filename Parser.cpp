#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"




const int Parser::priority_op[MaxPriority + 1][5] =
{ { assign }, { Plus, Minus, XOR, OR }, { Mul, Div, DIV, MOD, AND },
{ open_br, open_qbr, rec } };
const int Parser::priority_length[MaxPriority + 1] = { 1, 4, 5, 3 };

Parser::Parser()
{
	

}


Parser::Parser(const char* FileName) : T(FileName)
{
	root = NULL;
	dot_com_flag = false;
}

exception Parser::GetErrInformation(Lexeme l, string inf)
{
	char buff[20];
	exception e((string(_itoa(l.row, buff, 10)) + string(" ") +
	string(_itoa(l.col, buff, 10)) + string(" err ") + string(l.text) +
	string(" ") + inf).c_str());
	return e;
}


void Parser::Parse()
{
	if (T.End())
		return;
	root = ParseExpr(0, false);
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
		{
			dot_com_flag = false;
			break;
		}
		if (S.lexid == com)
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
	return (s == "char" || s == "integer" || s == "double"); //add new types
}

Expression* Parser::ParseAssign(Expression* l)
{
	if (!dot_com_flag)
		dot_com_flag = true;
	else
		throw GetErrInformation(LastLex, "double assign");
	if (l->type == IntConst || l->type == RealConst)
		throw GetErrInformation(LastLex, "assign const");
	return ParseExpr(1, NULL);
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
	Lexeme field = T.NextLex();
	if (field.type == TypeIdent)
		return new ExprVar(field.text);
	else
		throw GetErrInformation(field, "bad right operand"); //?
}

Expression* Parser::ParseFunction(int& operation_type, Expression* l)
{
	if (l->type != Var)
		throw GetErrInformation(LastLex, "bad function name");
	if (IsTypename(((ExprVar*)l)->name))
	{
		operation_type = Cast;
		return ParseCast();
	}
	operation_type = Func;
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
	return args;
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
		if (E->type == RealConst)
			throw GetErrInformation(LastLex, "bad array index");
		if (SaveLex.lexid == com)
			SaveLex.lexid = -1;
		else if (SaveLex.lexid == close_qbr)
		{
			if (E->type == Args)
				throw GetErrInformation(LastLex, "bad array index");
			args->args.push_back(E);
			break;
		}
		else
			throw GetErrInformation(SaveLex, "bad lex");
		args->args.push_back(E);
	}
	SaveLex.lexid = -1;
	return args;
}

Expression* Parser::ParseCast()
{
	Expression* E = ParseExpr(1, false);
	if (SaveLex.lexid != close_br)
		throw GetErrInformation(SaveLex, "bad lex");
	SaveLex.lexid = -1;
	return E;
}

Expression* Parser::ParseBinOp(int op_idx, int priority)
{
	return ParseExpr(priority + 1, NULL); //priority +1
}


Expression* Parser::ParseExpr(int priority, Expression* L)
{
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
		S = T.NextLex();
		LastLex = S;
		if (S.type == TypeEOF)
			return left;
		CheckSign(S);
	}
	else
	{
		S = SaveLex;
		SaveLex.lexid = -1;
	}
	int op_idx;
	Expression* right = NULL;
	for (int i = 0; i < priority_length[priority]; ++i)
	{
		if (S.lexid == priority_op[priority][i])
		{
			op_idx = priority_op[priority][i];
			if (IsSimpleOp)
				right = ParseBinOp(op_idx, priority);
			if (op_idx == open_qbr)
				right = ParseArray(left);
			if (op_idx == rec)
				right = ParseRecord(left);
			if (op_idx == assign)
				right = ParseAssign(left);
			if (op_idx == open_br)
			{
				int operation_type;
				right = ParseFunction(operation_type, left);
				op_idx = operation_type;
			}
			if (T.End())
				return new ExprBinOp(left, op_idx, right);
			return  ParseExpr(priority, new ExprBinOp(left, op_idx, right));
		}
	}
	SaveLex = S;
	return left;
}


Expression* Parser::ParseFactor()
{
	Lexeme lex = T.NextLex();
	if (LastLex.lexid == Minus && lex.lexid == Minus)
		throw GetErrInformation(lex, "bad minus");
	LastLex = lex;
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
		return new ExprUnOp(ParseExpr(MaxPriority, NULL), t); // ??? MaxPriority but its work
	CheckOperand(lex);
	if (lex.type == TypeInteger)
		return new ExprIntConst(stoi(lex.text));
	if (lex.type == TypeReal)
		return new ExprRealConst(stod(lex.text));
	if (lex.type == TypeIdent)
		return new ExprVar(lex.text);
	return NULL;
}

int Parser::IsUnaryOp(Lexeme l)   //add new operators
{
	if (l.lexid == Minus)
		return UnMinus; 
	if (l.lexid == dog)
		return dog;
	if (l.lexid == NOT)
		return NOT;
	return -1;          
}


void Parser::PrintNode(Expression* node, int h, FILE* F)
{
	int reindex_length = 14;
	static int reindex_array[] = {Func, Cast, open_qbr, assign, rec, Plus, 
	Minus, Mul, Div, DIV, MOD, OR, XOR, AND};
	static string print_array[] = { "F", "T", "[ ]", ":=", ".", "+",
	"-", "*", "/", "/", "MOD", "OR", "XOR", "AND"};
	static int first_arg_h = -1;
	if (node->right)	
		PrintNode(node->right, h + 1, F);
	if (first_arg_h == -1)
	{
		for (int i = 0; i < h * 4; ++i)
			fprintf_s(F, " ");
	}
	else
	{
		for (int i = 0; i < (h - first_arg_h) * 4; ++i)
			fprintf_s(F, " ");
		first_arg_h = -1;
	}
	int a = node->type;
	bool _break = false;
	switch (node->type)
	{
	case Var:
	{
		fprintf_s(F, "%s", ((ExprVar*)node)->name.c_str());
		break;
	}
	case IntConst:
	{
		fprintf_s(F, "%d", ((ExprIntConst*)node)->int_const);
		break;
	}
	case RealConst:
	{
		fprintf_s(F, "%.3lf", ((ExprRealConst*)node)->real_const);
		break;
	}
	case BinOp:
	{
		int op = ((ExprBinOp*)node)->operation;
		for (int i = 0; i < reindex_length; ++i)
		{
			if (reindex_array[i] == op)
			{
				fprintf_s(F, print_array[i].c_str());
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
			fprintf_s(F, "%s", string("***\n\n").c_str());
			break;
		}
		ExprArgs* n = ((ExprArgs*)node);
		int l = n->args.size();
		for (int i = 0; i < l; ++i)
		{
			if (i == 0)
			{
				first_arg_h = h;
				PrintNode(n->args.at(i), h, F);
			}
			else
				PrintNode(n->args.at(i), h, F);
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
			fprintf_s(F, "%s", string("-").c_str());
			break;
		}
		case NOT:
		{
			_break = true;
			fprintf_s(F, "%s", string("NOT").c_str());
			break;
		}
		case dog:
		{
			_break = true;
			fprintf_s(F, "%s", string("@").c_str());
			break;
		}
		}
	}
	}
	if (node->type != Args)
		fprintf(F, "\n\n");
	if (node->left)
		PrintNode(node->left, h + 1, F);
	return;
}

void Parser::PrintTree(string FileName)
{
	if (root == NULL)
		return;
	FILE* F;
	fopen_s(&F, FileName.c_str(), "wt");
	PrintNode(root, 0, F);
	fclose(F);
	return;
}

Parser::~Parser()
{
	delete root;
}
