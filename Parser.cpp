#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"

Parser::Parser(const char* FileName) : T(FileName), FileNotFound(T.End())
{
	except = false;
	BracketCounter = 0;
	root = NULL;
	names = NULL;
	values = NULL;
	numVars = 0;
	SaveLex.text = "";
}

bool Parser::getException()
{
	return except;
}


void Parser::Parse()
{
	if (T.End())
		return;
	root = ParseExpr(NULL);
	if (BracketCounter > 0)
	{
		char buff[20];
		Lexeme L = T.NextLex();
		printf("\n");
		printf_s((string(_itoa(L.row, buff, 10)) + string(" ") + string(_itoa(L.col, buff, 10)) +
			string(" expected \')\'")).c_str());
		printf_s("\n\n");
		except = true;
	}
	return;
}

bool Parser::CheckSign(Lexeme S)
{
	try
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
		case TypeSep:
		{
			if (S.text == ")")
			{
				if (BracketCounter == 0)
				{
					exception a((string(_itoa(S.row, buff, 10)) + string(" ") + string(_itoa(S.col, buff, 10)) +
						string(" bad bracket")).c_str());
					throw a;
				}
				else
					break;
			}
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
	}
	catch (exception a)
	{
		except = true;
		printf_s("\n%s\n\n", a.what());
		return true;
	}
	return false;
}

bool Parser::CheckOperand(Lexeme S)
{
	try
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
		case TypeFalse:
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
	}
	catch (exception a)
	{
		except = true;
		printf_s("\n%s\n\n", a.what());
		return true;
	}
	return false;
}

Expression* Parser::ParseExpr(Expression* l)
{
	Expression* left = l;
	if (left == NULL)
		left = ParseTerm(NULL);
	if (except)
		return NULL;
	Lexeme S;
	if (SaveLex.text == "")
	{
		S = T.NextLex();
		if (S.type == TypeFalse)
			return left;
		if (CheckSign(S))
			return NULL;
	}
	else
	{
		S = SaveLex;
		SaveLex.text = "";
	}
	if (S.text != "+" && S.text != "-")
	{
		SaveLex = S;
		return left;
	}
	Expression* right = ParseTerm(NULL);
	if (except)
		return NULL;
	int t;
	if (S.text == "+")
		t = Add;
	else
		t = Sub;
	if (T.End())
		return new ExprBinOp(left, t, right);
	return ParseExpr(new ExprBinOp(left, t, right));
}

Expression* Parser::ParseTerm(Expression* l)
{
	Expression* left = l;
	if (left == NULL)
		left = ParseFactor();
	if (except)
		return NULL;
	Lexeme S;
	if (SaveLex.text == "")
	{
		S = T.NextLex();
		if (S.type == TypeFalse)
			return left;
		if (CheckSign(S))
			return NULL;
	}
	else
	{
		S = SaveLex;
		SaveLex.text = "";
	}
	if (S.text != "*" && S.text != "/")
	{
		SaveLex = S;
		return left;
	}
	Expression* right = ParseFactor();
	if (except)
		return NULL;
	int t;
	if (S.text == "*")
		t = Mul;
	else
		t = Div;
	if (T.End())
		return new ExprBinOp(left, t, right);
	return ParseTerm(new ExprBinOp(left, t, right));
}

Expression* Parser::ParseFactor()
{
	Lexeme lex = T.NextLex();
	if (lex.text == "(")
	{
		++BracketCounter;
		return Brackets();
	}
	if (CheckOperand(lex))
		return NULL;
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
	l = ParseExpr(NULL);
	if (SaveLex.text == ")")
	{
		SaveLex = SaveLexSave;
		--BracketCounter;
		return l;
	}
	return NULL;
}

void Parser::DestroyNode(Expression* node)
{
	if (node == NULL)
		return;
	DestroyNode(node->left);
	DestroyNode(node->right);
	delete node;
	return;
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
	if (node->type == Add)
		return Calc(node->left) + Calc(node->right);
	if (node->type == Sub)
		return Calc(node->left) - Calc(node->right);
	if (node->type == Mul)
		return Calc(node->left) * Calc(node->right);
	if (node->type == Div)
		return Calc(node->left) / Calc(node->right);
}

double Parser::CalcTree()
{
	if (root != NULL)
		return (Calc(root));
	else
		return 0;
}


void Parser::PrintNode(Expression* node, int h, FILE* F)
{
	static char sign[4]; sign[0] = '+'; sign[1] = '-'; sign[2] = '*'; sign[3] = '/';
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
	DestroyNode(root);
}
