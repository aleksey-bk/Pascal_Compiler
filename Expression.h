#pragma once
#include "SymType.h"
#include "Symbol.h"
#include <cstdlib>
#include <string>
#include <vector>

enum { UnMinus, VoidArg, Func, Cast, Cap, Dog, Not};
enum { Args, BinOp, IntConst, RealConst, CharConst, UnOp, Var};

using namespace std;

class Expression
{
public:
	Expression* right;
	Expression* left;
	int type;
	SymType* expr_type;
	Expression();
	~Expression();
};

