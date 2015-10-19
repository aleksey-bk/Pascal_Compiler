#pragma once
#include <cstdlib>
#include <string>
#include <vector>

enum { Var, RealConst, IntConst, UnMinus, Args, TypeCast, TypeCastName };

using namespace std;

class Expression
{
public:
	int type;
	Expression* right;
	Expression* left;
	int intConst;
	double realConst;
	string name;
	vector<Expression*> args;
	Expression();
	~Expression();
};

