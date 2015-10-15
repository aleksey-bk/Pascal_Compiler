#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"

const int Parser::priority_op[6][2] = { assign, 0, Plus, Minus, Mul, Div, open_br, 0, open_qbr, 0, rec, 0 };
const int Parser::priority_length[6] = { 1, 2, 2, 1, 1, 1 };


Parser::Parser()
{

}


Parser::Parser(const char* FileName) : T(FileName)
{
	root = NULL;
	names = NULL;
	values = NULL;
	numVars = 0;
	SaveLex.text = "";
}


void Parser::Parse()
{
	if (T.End())
		return;
	root = ParseExpr(NULL, 0);
	return;
}

bool Parser::CheckSign(Lexeme S)
{
	char buff[20];
	switch (S.type)
	{
	case TypeErr:
	{
		exception a((string(_itoa(S.row, buff, 10)) + string(" ") + string(_itoa(S.col, buff, 10)) +
		string(" err ") + S.text).c_str());
		throw a;
	}
	default:
	{
		if (S.type == TypeOp)
			break;
		exception a((string(_itoa(S.row, buff, 10)) + string(" ") + string(_itoa(S.col, buff, 10)) +
			string(" bad lex \"") + S.text + string("\"")).c_str());
		throw a;
	}
	}
	return false;
}

bool Parser::CheckOperand(Lexeme S)
{
		char buff[20];
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
			exception a((string(_itoa(S.row, buff, 10)) + string(" ") + string(_itoa(S.col, buff, 10)) +
			string(" err \"") + S.text + string("\"")).c_str());
			throw a;
		}
		case TypeEOF:
		{
			exception a((string(_itoa(S.row, buff, 10)) + string(" ") + string(_itoa(S.col, buff, 10)) +
			string(" BAD EOF")).c_str());
			throw a;
		}
		default:
		{
			exception a((string(_itoa(S.row, buff, 10)) + string(" ") + string(_itoa(S.col, buff, 10)) + 
			string(" unexpected lex \"") + S.text + string("\"")).c_str());
			throw a;
		}
	}
	return false;
}



Expression* Parser::ParseExpr(Expression* l, int priority)
{
	if (priority == ParseF)
		return ParseFactor();
	Expression* left = l;
	if (left == NULL)
		left = ParseExpr(NULL, priority + 1);
	Lexeme S;
	if (SaveLex.text == "")
	{
		S = T.NextLex();
		if (S.type == TypeEOF)
			return left;
		CheckSign(S);
	}
	else
	{
		S = SaveLex;
		SaveLex.text = "";
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
	switch (priority)
	{
	case FuncP:
	{

	}
	case ArrP:
	{

	}
	case RecP:
	{
		
	}
	}
	Expression* right = ParseExpr(NULL, priority + 1); 
	if (T.End())
		return new ExprBinOp(left, op_idx, right);
	return ParseExpr(new ExprBinOp(left, op_idx, right), priority);
}


Expression* Parser::ParseFactor()
{
	Lexeme lex = T.NextLex();
	if (lex.lexid == open_br)
	{
		//return Brackets();
		Expression* r = ParseExpr(NULL, 0);
		SaveLex.text = "";
		return r;
	}
	CheckOperand(lex);
	if (lex.type == TypeInteger)
		return new ExprIntConst(stoi(lex.text));
	if (lex.type == TypeReal)
		return new ExprRealConst(stod(lex.text));
	if (lex.type == TypeIdent)
		return new ExprVar(lex.text);
}

Expression* Parser::Brackets()
{
	Lexeme SaveLexSave = SaveLex;
	Expression* l;
	l = ParseExpr(NULL, 0);
	if (SaveLex.text != ")")
	{
		char buff[20];
		exception a((string("\n") + string(_itoa(SaveLex.row, buff, 10)) + string(_itoa(SaveLex.col, buff, 10)) +
			string(" expected \')\'") + string("\n\n")).c_str());
		throw a;
	}
	SaveLex = SaveLexSave;
	return l;
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
	static string sign[10]; sign[0] = "+"; sign[1] = "-"; sign[2] = '*'; sign[3] = "/"; //____
	sign[6] = "[]"; sign[7] = ".";
	if (node->left)
		PrintNode(node->left, h + 1, F);
	for (int i = 0; i < h * 4; ++i)
		fprintf_s(F, " ");
	int a = node->type;
	switch (node->type)
	{
	case Var:
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
	default:
	{
		fprintf_s(F, "%c", sign[node->type - 1]);
		break;
	}
	}
	fprintf(F, "\n\n");
	if (node->right)
		PrintNode(node->right, h + 1, F);
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
