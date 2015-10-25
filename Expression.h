#pragma once
#include <cstdlib>
#include <string>
#include <vector>

enum { UnMinus, VoidArg, Func, Cast };
enum { Args, BinOp, IntConst, RealConst, UnOp, Var };

using namespace std;

class Expression
{
public:
	Expression* right;
	Expression* left;
	int type;
	Expression();
	~Expression();
};

