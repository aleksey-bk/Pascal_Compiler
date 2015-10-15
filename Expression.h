#pragma once
#include <cstdlib>
#include <string>

enum { Var, RealConst, IntConst };

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
	Expression();
	~Expression();
};

