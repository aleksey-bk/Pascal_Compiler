#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"

const int Parser::priority_op[MaxPriority + 1][3] = { assign, 0, 0, Plus, Minus, 0, Mul, Div, 0, open_br, open_qbr, rec };
const int Parser::priority_length[MaxPriority + 1] = { 1, 2, 2, 3};



Parser::Parser()
{
	

}


Parser::Parser(const char* FileName) : T(FileName)
{
	root = NULL;
	names = NULL;
	values = NULL;
	dot_com_flag = false;
	numVars = 0;
}

exception Parser::GetErrInformation(Lexeme l, string inf)
{
	char buff[20];
	exception e((string(_itoa(l.row, buff, 10)) + string(" ") +
	string(_itoa(l.col, buff, 10)) + string(" err ") + string(l.text) +
	string(" ") + inf).c_str());
	fclose(T.F);
	return e;
}


void Parser::Parse()
{
	if (T.End())
		return;
	root = ParseExpr(NULL, FormNoBr, 0);
	return;
}

void Parser::CheckSign(Lexeme S, int form)
{
	char buff[20];
	switch (S.type)
	{
	case TypeErr:
	{
		throw GetErrInformation(S, "");
	}
	case TypeOp:
	{
		if ((S.lexid == close_br) && (form != FormExpr && form != FormFunc))
			throw GetErrInformation(S, "unexpected lex");
		if ((S.lexid == close_qbr) && (form != FormArr))
			throw GetErrInformation(S, "unexpected lex");
		if (S.lexid == assign && dot_com_flag == true)
			throw GetErrInformation(S, "unexpected lex");
		break;
	}
	case TypeSep:
	{
		if ((S.lexid == com) && (form == FormFunc || form == FormArr))
			break;
		if (S.lexid == dot_com)
		{
			dot_com_flag = false;
			break;
		}
		throw GetErrInformation(S, "unexpected lex");
	}
	default:
	{
		throw GetErrInformation(S, "bad lex");
	}
	}
	return;
}

void Parser::CheckLeft(Lexeme S, Expression* left)
{
	if ((S.lexid == assign || S.lexid == rec || S.lexid == open_qbr)
		&& (left->type != Var && left->type != open_qbr && left->type != rec && left->type != open_br))
		throw GetErrInformation(S, "bad left operand");
	if (!dot_com_flag && S.lexid == assign)
		dot_com_flag = true;
	return;
}

void Parser::CheckOperand(Lexeme S)
{
		switch (S.type)
		{
		case TypeInteger :
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


Expression* Parser::ParseArgs(int id_end_parse)
{
	int form;
	vector<Expression*> R;
	if (id_end_parse == close_br)
		form = FormFunc;
	else
		form = FormArr;
	bool set_dcf = true;
	if (dot_com_flag)
		set_dcf = false;
	else
	{
		dot_com_flag = true;
		set_dcf = true;
	}
	while (true)
	{
		if (T.End())
		{
			if (id_end_parse == close_br)
				throw GetErrInformation(SaveLex, "expected \")\"");
			else
				throw GetErrInformation(SaveLex, "expected \"]\"");
		}
		Expression* E = ParseExpr(NULL, form, 0);
		if (SaveLex.lexid == com)
			SaveLex.lexid = -1;
		if (SaveLex.lexid == id_end_parse)
		{
			R.push_back(E);
			SaveLex.lexid = -1;
			break;
		}
		R.push_back(E);
	}
	if (set_dcf)
		dot_com_flag = false;
	ExprArgs* ret = new ExprArgs();
	ret->args = R;
	return ret;
}

bool Parser::IsTypename(Lexeme l)
{
	return (l.text == "char" || l.text == "integer" || l.text == "double"); //add new types
}


Expression* Parser::ParseExpr(Expression* l, int form, int priority)
{
	if (priority == ParseF)
		return ParseFactor(form);
	Expression* left = l;
	if (left == NULL)
		left = ParseExpr(NULL, form, priority + 1);
	Lexeme S;
	if (SaveLex.lexid == -1)
	{
		S = T.NextLex();
		if (S.type == TypeEOF)
			return left;
		CheckSign(S, form);
		CheckLeft(S, left);
	}
	else
	{
		S = SaveLex;
		SaveLex.lexid = -1;
	}
	bool ret = true;
	int op_idx;
	for (int i = 0; i < priority_length[priority]; ++i)
		if (S.lexid == priority_op[priority][i])
		{
			ret = false;
			op_idx = priority_op[priority][i];
		}
	if (ret == true)
	{
		SaveLex = S;
		return left;
	}
	Expression* right = NULL;
	if (S.lexid == open_br)
		right = ParseArgs(close_br);
	if (S.lexid == open_qbr)
		right = ParseArgs(close_qbr);
	if (right == NULL)
		right = ParseExpr(NULL, form, priority + 1); // priority + 1
	if (T.End())
		return new ExprBinOp(left, op_idx, right);
	return ParseExpr(new ExprBinOp(left, op_idx, right), form, priority); //priority 
}


Expression* Parser::ParseFactor(int form)
{
	Lexeme lex = T.NextLex();
	if (lex.lexid == open_br)
	{
		Expression* r = ParseExpr(NULL, FormExpr, 0);
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
		if (t == UnMinus && form == FormUnMinus)
			throw GetErrInformation(lex, "bad minus");
		Expression* r = NULL;
		if (t == TypeCast)
		{
			r = new Expression();
			r->name = lex.text;
			r->type = TypeCastName;
			return new ExprUnOp(ParseFactor(form), t, r);
		}
		//for debug delete this________
		if (t == UnMinus)
		{
			r = new Expression();
			r->name = string("***");
			r->type = Var;
		}
		//_____________________________
		return new ExprUnOp(ParseFactor(FormUnMinus), t, r);
	}
	CheckOperand(lex);
	if (lex.type == TypeInteger)
		return new ExprIntConst(stoi(lex.text));
	if (lex.type == TypeReal)
		return new ExprRealConst(stod(lex.text));
	if (lex.type == TypeIdent)
		return new ExprVar(lex.text);
}

int Parser::IsUnaryOp(Lexeme l)   //add new operators
{
	if (l.lexid == Minus)
		return UnMinus; 
	if (IsTypename(l))
		return TypeCast;
	return -1;          
}


void Parser::AddVar(double val, string name)
{
	numVars += 1;
	if (values == NULL)
	{
		values = (double*)calloc(1, sizeof(double));
		names = (string*)calloc(1, sizeof(string));
	}
	else
	{
		realloc(values, numVars * sizeof(double));
		realloc(values, numVars * sizeof(string));
	}
	values[numVars - 1] = val;
	names[numVars - 1] = name;
	return;
}

double Parser::Calc(Expression* node)
{
	if (node->type == IntConst)
		return node->intConst;
	if (node->type == RealConst)
		return node->realConst;
	if (node->type == Var)
	{
		for (int i = 0; i < numVars; ++i)
			if (names[i] == node->name)
				return values[i];
		double var;
		printf("\nVar \'%s\' := ", node->name.c_str());
		scanf_s("%lf", &var);
		AddVar(var, node->name);
		return var;
	}
	if (node->type == Plus)
		return Calc(node->left) + Calc(node->right);
	if (node->type == UnMinus)
		return -Calc(node->left);
	if (node->type == Minus)
		return Calc(node->left) - Calc(node->right);
	if (node->type == Mul)
		return Calc(node->left) * Calc(node->right);
	if (node->type == Div)
		return Calc(node->left) / Calc(node->right);
}

double Parser::CalcTree()
{
	if (root != NULL)
		return Calc(root);
	else
		return 0;
}


void Parser::PrintNode(Expression* node, int h, FILE* F)
{
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
	switch (node->type)
	{
	case Var:
	{
		fprintf_s(F, "%s", node->name.c_str());
		break;
	}
	case TypeCastName:
	{
		fprintf_s(F, "%s", node->name.c_str());
		break;
	}
	case IntConst:
	{
		fprintf_s(F, "%d", node->intConst);
		break;
	}
	case RealConst:
	{
		fprintf_s(F, "%.3lf", node->realConst);
		break;
	}
	case open_br:
	{
		fprintf_s(F, "%s", string("(f)").c_str());
		break;
	}
	case open_qbr:
	{
		fprintf_s(F, "%s", string("[a]").c_str());
		break;
	}
	case assign:
	{
		fprintf_s(F, "%s", string(":=").c_str());
		break;
	}
	case rec:
	{
		fprintf_s(F, "%c", '.');
		break;
	}
	case Plus:
	{
		fprintf_s(F, "%c", '+');
		break;
	}
	case Minus:
	{
		fprintf_s(F, "%c", '-');
		break;
	}
	case Mul:
	{
		fprintf_s(F, "%c", '*');
		break;
	}
	case Div:
	{
		fprintf_s(F, "%c", '/');
		break;
	}
	case Args:
	{
		int l = node->args.size();
		for (int i = 0; i < l; ++i)
		{
			if (i == 0)
			{
				first_arg_h = h;
				PrintNode(node->args.at(i), h, F);
			}
			else
				PrintNode(node->args.at(i), h, F);
		}
		break;
	}
	case TypeCast:
	{
		fprintf_s(F, "%s", string("(t)").c_str());
		break;
	}
	case UnMinus:
	{
		fprintf_s(F, "%s", string("-").c_str());
		break;
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
