#pragma once
#include <cstdlib>
#include <string>
#include <vector>

enum { Ident, RealConst, IntConst, UnMinus, Args, Cast, CastName, VoidArg };

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

